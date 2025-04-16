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

        EthernetHeader& GetEthernetHeader();

        IPv4Header& GetIPv4Header();

        IPv6Header& GetIPv6Header();

        TCPHeader& GetTCPHeader();

        UDPHeader& GetUDPHeader();

        [[nodiscard]] std::string GetTime() const;

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
        timeval time;

    };

    static std::string mac_to_string(const uint8_t mac[6]);


    inline std::ostream& operator<<(std::ostream& os, const EthernetHeader& header);

    std::ostream& operator<<(std::ostream& os, const IPv4Header& header);

    std::ostream& operator<<(std::ostream& os, const IPv6Header& header);

    std::ostream& operator<<(std::ostream& os, const TCPHeader& header);

    std::ostream& operator<<(std::ostream& os, const UDPHeader& header);

    std::string timeval_to_datetime_string(const struct timeval& tv, const std::string& format = "%Y-%m-%d %H:%M:%S.%f");

}

#endif //PACKET_H
