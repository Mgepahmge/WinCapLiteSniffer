//
// Created by Mgepahmge on 25-4-14.
//
#include "ParseTransport.h"

void wcls::ParseTCP(const u_char* packet, const TCPHeader* header, uint32_t caplen) {
    if (caplen < sizeof(TCPHeader)) {
        return;
    }
    header = reinterpret_cast<const TCPHeader*>(packet);
}

void wcls::ParseUDP(const u_char* packet, const UDPHeader* header, uint32_t caplen) {
    if (caplen < sizeof(UDPHeader)) {
        return;
    }
    header = reinterpret_cast<const UDPHeader*>(packet);
}
