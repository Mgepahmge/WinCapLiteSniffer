//
// Created by Mgepahmge on 25-4-14.
//

#ifndef PACKET_H
#define PACKET_H
#include <iomanip>
#include <sstream>
#include <winsock2.h>
#include "utils.h"

namespace wcls {
    class Packet {
    public:
        friend std::ostream& operator<<(std::ostream& os, const Packet& packet);

        Packet(const u_char* data, const struct pcap_pkthdr* pkthdr);
        ~Packet();
    private:
        const u_char* data;
        const struct pcap_pkthdr* pkthdr;
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

    static std::string mac_to_string(const uint8_t mac[6]);


    inline std::ostream& operator<<(std::ostream& os, const EthernetHeader& header);

    std::ostream& operator<<(std::ostream& os, const IPv4Header& header);

    std::ostream& operator<<(std::ostream& os, const IPv6Header& header);

    std::ostream& operator<<(std::ostream& os, const TCPHeader& header);

    std::ostream& operator<<(std::ostream& os, const UDPHeader& header);

}

#endif //PACKET_H
