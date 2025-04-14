//
// Created by Mgepahmge on 25-4-14.
//

#ifndef PARSETRANSPORT_H
#define PARSETRANSPORT_H
#include <cstdint>
#include <winsock2.h>

#include "Protocols/TransportLayer.h"

namespace wcls {
    using namespace protocol;

    void ParseTCP(const u_char* packet, const TCPHeader* header, uint32_t caplen);

    void ParseUDP(const u_char* packet, const UDPHeader* header, uint32_t caplen);
}

#endif //PARSETRANSPORT_H
