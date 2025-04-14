//
// Created by Mgepahmge on 25-4-14.
//
#include "Packet.h"

wcls::Packet::Packet(u_char* data, struct pcap_pkthdr* pkthdr) : data(data), pkthdr(pkthdr), ethernetHeader(),
                                                                 ipv4Header(), ipv6Header(), tcpHeader(), udpHeader(),
                                                                 ethernetEnable(false), ipv4Enable(false),
                                                                 ipv6Enable(false), tcpEnable(false),
                                                                 udpEnable(false) {
    uint32_t caplen = pkthdr->caplen;
    const u_char* temp = data;
    temp = ParseEthernet(temp, &ethernetHeader, caplen);
    if (temp == nullptr) {
        return;
    }
    ethernetEnable = true;
    if (ethernetHeader.vlanInfo ? ethernetHeader.vlanEtherType : ethernetHeader.etherType == MY_ETHERTYPE_IP) {
        temp = ParseIpv4(temp, &ipv4Header, caplen);
        if (temp == nullptr) {
            return;
        }
        ipv4Enable = true;
        switch (ipv4Header.protocol) {
            case MY_IPPROTO_TCP: {
                tcpEnable = ParseTCP(temp, &tcpHeader, caplen);
                break;
            }
            case MY_IPPROTO_UDP: {
                udpEnable = ParseUDP(temp, &udpHeader, caplen);
                break;
            }
            default:
                break;
        }
    } else if (ethernetHeader.vlanInfo ? ethernetHeader.vlanEtherType : ethernetHeader.etherType == MY_ETHERTYPE_IPV6) {
        temp = ParseIpv6(temp, &ipv6Header, caplen);
        if (temp == nullptr) {
            return;
        }
        ipv6Enable = true;
        switch (ipv6Header.next_header) {
            case MY_IPPROTO_TCP: {
                tcpEnable = ParseTCP(temp, &tcpHeader, caplen);
                break;
            }
            case MY_IPPROTO_UDP: {
                udpEnable = ParseUDP(temp, &udpHeader, caplen);
                break;
            }
            default:
                break;
        }
    }
}

wcls::Packet::~Packet() {
    delete[] data;
    delete[] pkthdr;
}
