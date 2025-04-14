//
// Created by Mgepahmge on 25-4-14.
//
#include "ParseTransport.h"

bool wcls::ParseTCP(const u_char* packet, TCPHeader* header, uint32_t caplen) {
    if (caplen < sizeof(TCPHeader)) {
        return false;
    }
    memcpy(header, packet, sizeof(TCPHeader));
    return true;
}

bool wcls::ParseUDP(const u_char* packet, UDPHeader* header, uint32_t caplen) {
    if (caplen < sizeof(UDPHeader)) {
        return false;
    }
    memcpy(header, packet, sizeof(UDPHeader));
    return true;
}
