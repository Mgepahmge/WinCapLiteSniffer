//
// Created by Mgepahmge on 25-4-14.
//
#include "ParseTransport.h"

void wcls::ParseTCP(const u_char* packet, TCPHeader* header, uint32_t caplen) {
    if (caplen < sizeof(TCPHeader)) {
        return;
    }
    memcpy(header, packet, sizeof(TCPHeader));
}

void wcls::ParseUDP(const u_char* packet, UDPHeader* header, uint32_t caplen) {
    if (caplen < sizeof(UDPHeader)) {
        return;
    }
    memcpy(header, packet, sizeof(UDPHeader));
}
