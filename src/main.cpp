#include <iostream>
#include <pcap.h>
#include <ws2tcpip.h>
#include <winsock2.h>

int main()
{
    pcap_if_t* alldevs;
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cerr << "Error finding devices: " << errbuf << std::endl;
        return 1;
    }
    std::cout << "Available network devices:\n";
    for (pcap_if_t* d = alldevs; d != nullptr; d = d->next) {
        std::cout << "- " << d->name;
        if (d->description)
            std::cout << " (" << d->description << ")";
        std::cout << std::endl;
        for (pcap_addr_t* addr = d->addresses; addr; addr = addr->next) {
            if (addr->addr && addr->addr->sa_family == AF_INET) {
                char ipstr[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &((sockaddr_in*)addr->addr)->sin_addr,
                         ipstr, sizeof(ipstr));
                std::cout << "  IP: " << ipstr << std::endl;
            }
        }

    }

    pcap_freealldevs(alldevs);
    return 0;
}
