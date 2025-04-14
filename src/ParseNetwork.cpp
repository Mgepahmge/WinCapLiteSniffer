//
// Created by Mgepahmge on 25-4-14.
//
#include "ParseNetwork.h"

const u_char* wcls::ParseIpv4(const u_char* packet, const IPv4Header* header, bool& is_tcp, bool& is_udp, uint32_t caplen) {
    if (caplen < sizeof(IPv4Header)) {
        return nullptr;
    }
    header = reinterpret_cast<const IPv4Header*>(packet);
    if (header->protocol == 6) {
        is_tcp = true;
    }
    if (header->protocol == 17) {
        is_udp = true;
    }
    const uint8_t ihl = (header->version_ihl & 0x0F) * 4;
    return packet + ihl;
}
