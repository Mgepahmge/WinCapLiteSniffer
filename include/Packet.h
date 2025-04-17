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
        bool ethernetEnable;
        bool ipv4Enable;
        bool ipv6Enable;
        bool tcpEnable;
        bool udpEnable;

        friend std::ostream& operator<<(std::ostream& os, const Packet& packet);

        Packet(const u_char* data, const struct pcap_pkthdr* pkthdr);

        ~Packet();

        [[nodiscard]] EthernetHeader GetEthernetHeader() const;

        [[nodiscard]] IPv4Header GetIPv4Header() const;

        [[nodiscard]] IPv6Header GetIPv6Header() const;

        [[nodiscard]] TCPHeader GetTCPHeader() const;

        [[nodiscard]] UDPHeader GetUDPHeader() const;

        [[nodiscard]] std::string GetTime() const;

        [[nodiscard]] uint32_t GetPacketLength() const;

    private:
        const u_char* data;
        const struct pcap_pkthdr* pkthdr;
        EthernetHeader ethernetHeader;
        IPv4Header ipv4Header;
        IPv6Header ipv6Header;
        TCPHeader tcpHeader;
        UDPHeader udpHeader;
        timeval time;

    };

    static std::string mac_to_string(const uint8_t mac[6]) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (int i = 0; i < 6; ++i) {
            ss << std::setw(2) << static_cast<int>(mac[i]);
            if (i != 5) ss << ":";
        }
        return ss.str();
    }


    inline std::ostream& operator<<(std::ostream& os, const EthernetHeader& header);

    std::ostream& operator<<(std::ostream& os, const IPv4Header& header);

    std::ostream& operator<<(std::ostream& os, const IPv6Header& header);

    std::ostream& operator<<(std::ostream& os, const TCPHeader& header);

    std::ostream& operator<<(std::ostream& os, const UDPHeader& header);

    std::string timeval_to_datetime_string(const struct timeval& tv, const std::string& format = "%Y-%m-%d %H:%M:%S.%f");

}

#endif //PACKET_H
