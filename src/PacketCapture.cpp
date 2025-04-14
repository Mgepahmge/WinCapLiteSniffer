//
// Created by Mgepahmge on 25-4-13.
//
#include "PacketCapture.h"

#include <utility>

namespace wcls {
    PacketCapture::PacketCapture() : m_pcapHandle(NULL),  m_promiscuous(true), m_timeout(1000),
                                     m_snaplen(65535), m_errBuf{} {
    }

    PacketCapture& PacketCapture::Instance() {
        static PacketCapture instance;
        return instance;
    }

    PacketCapture::~PacketCapture() {
        if (m_pcapHandle != NULL) {
            pcap_close(m_pcapHandle);
        }
    }

    bool PacketCapture::Initialize() {
        if (m_pcapHandle != NULL) {
            pcap_close(m_pcapHandle);
        }
        m_pcapHandle = pcap_open_live(m_device.c_str(), m_snaplen, m_promiscuous, m_timeout, m_errBuf);

        if (!m_pcapHandle) {
            throw std::runtime_error(m_errBuf);
        }
        if (pcap_datalink(m_pcapHandle) != DLT_EN10MB) {
            throw std::runtime_error("Only Ethernet networks are supported");
        }
        return true;
    }

    bool PacketCapture::StartCapture() {
        if (!m_pcapHandle || m_captureRunning) return false;

        m_captureRunning = true;
        m_captureThread = std::thread([this]() {
            auto ret = pcap_loop(m_pcapHandle, 0, PacketHandler, nullptr);
            m_captureRunning = false;
            if (ret == -2) {
                printf("pcap_loop interrupted\n");
            } else if (ret == -1) {
                printf("pcap_loop error: %s\n", pcap_geterr(m_pcapHandle));
            }
        });
        return true;
    }

    void PacketCapture::StopCapture() {
        if (m_captureRunning && m_pcapHandle) {
            pcap_breakloop(m_pcapHandle);
            if (m_captureThread.joinable()) {
                m_captureThread.join();
            }
        }
    }

    void PacketCapture::SetDevice(const std::string& deviceName) {
        m_device = deviceName;
    }

    void PacketCapture::SetPromiscuous(const bool enable) {
        m_promiscuous = enable;
    }

    void PacketCapture::SetTimeout(const int milliseconds) {
        m_timeout = milliseconds;
    }

    void PacketCapture::SetMaxPacketSize(const int size) {
        m_snaplen = size;
    }

    void PacketCapture::PacketHandler(u_char* userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
        printf("Captured packet length: %d\n", pkthdr->len);
        const EthernetHeader header{};
        bool is_ipv4 = false;
        const u_char* data = ParseEthernet(packet, &header, is_ipv4, pkthdr->len);
        printf("Parsed Ethernet");
        if (is_ipv4) {
            printf("Parsed IPv4");
            const IPv4Header ipHeader{};
            bool is_tcp = false;
            bool is_udp = false;
            data = ParseIpv4(data, &ipHeader, is_tcp, is_udp, pkthdr->len - sizeof(EthernetHeader));
            if (is_tcp) {
                printf("Parsed TCP");
                const TCPHeader tcpHeader{};
                ParseTCP(data, &tcpHeader, pkthdr->len - sizeof(EthernetHeader) - sizeof(IPv4Header));
            } else if (is_udp) {
                printf("Parsed UDP");
                const UDPHeader udpHeader{};
                ParseUDP(data, &udpHeader, pkthdr->len - sizeof(EthernetHeader) - sizeof(IPv4Header));
            }
        }
    }
}



