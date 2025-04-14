//
// Created by Mgepahmge on 25-4-14.
//

#ifndef PACKET_H
#define PACKET_H
#include <winsock2.h>
#include "utils.h"

namespace wcls {
    class Packet {
    public:
        Packet(u_char* data, struct pcap_pkthdr* pkthdr);
        ~Packet();
    private:
        u_char* data;
        struct pcap_pkthdr* pkthdr;
        EthernetHeader ethernetHeader;
        IPv4Header ipv4Header;
        IPv6Header ipv6Header;
        TCPHeader tcpHeader;
        UDPHeader udpHeader;
        bool ethernetEnable;
        bool ipv4Enable;
        bool ipv6Enable;
        bool tcpEnable;
        bool udpEnable;

    };
}

#endif //PACKET_H
