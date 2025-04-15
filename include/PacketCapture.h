//
// Created by Mgepahmge on 25-4-13.
//

#ifndef PACKETCAPTURE_H
#define PACKETCAPTURE_H
#include "Packet.h"

namespace wcls {
    class PacketCapture {
    public:
        PacketCapture(const PacketCapture&) = delete;
        PacketCapture& operator=(const PacketCapture&) = delete;
        static PacketCapture& Instance();
        ~PacketCapture();
        bool Initialize();
        bool StartCapture();
        size_t StopCapture();
        void SetDevice(const std::string& deviceName);
        void SetPromiscuous(bool enable);
        void SetTimeout(int milliseconds);
        void SetMaxPacketSize(int size);
        void addPacket(const Packet& packet);
        std::vector<Packet>& getPackets();
    private:
        std::thread m_captureThread;
        std::atomic<bool> m_captureRunning{false};
        pcap_t* m_pcapHandle;
        std::string m_device;
        bool m_promiscuous;
        int m_timeout;
        int m_snaplen;
        char m_errBuf[PCAP_ERRBUF_SIZE];
        std::vector<Packet> m_packets;

        PacketCapture();

        static void PacketHandler(u_char* userData, const struct pcap_pkthdr* pkthdr, const u_char* packet);
    };
}

#endif //PACKETCAPTURE_H
