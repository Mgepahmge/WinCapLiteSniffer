//
// Created by Mgepahmge on 25-4-14.
//
#include "ParseEthernet.h"

const u_char* wcls::ParseEthernet(const u_char* packet, const EthernetHeader* header, bool& is_ipv4, uint32_t caplen) {
    if (caplen < sizeof(EthernetHeader)) {
        return nullptr;
    }
    header = reinterpret_cast<const EthernetHeader*>(packet);
    is_ipv4 = ntohs(header->etherType) == 0x0800;
    return packet + sizeof(EthernetHeader);
}
