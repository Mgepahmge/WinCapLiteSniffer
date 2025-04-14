//
// Created by Mgepahmge on 25-4-14.
//
#include "ParseNetwork.h"

const u_char* wcls::ParseIpv4(const u_char* packet, IPv4Header* header, uint32_t& caplen) {
    if (caplen < 20) {
        return nullptr;
    }

    header->version_ihl = *packet;
    uint8_t ihl = (header->version_ihl & 0x0F) * 4;

    if (ihl < 20 || caplen < ihl) {
        return nullptr;
    }

    memcpy(header, packet, sizeof(IPv4Header));

    caplen -= ihl;
    return packet + ihl;
}

const u_char* wcls::ParseIpv6(const u_char* packet, IPv6Header* header, uint32_t& caplen) {
    if (caplen < 40) {
        return nullptr;
    }

    memcpy(header, packet, sizeof(IPv6Header));

    if (((header->version_class_flow >> 28) & 0x0F) != 6) {
        return nullptr;
    }

    caplen -= 40;
    return packet + 40;
}
