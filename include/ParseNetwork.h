//
// Created by Mgepahmge on 25-4-14.
//

#ifndef PARSENETWORK_H
#define PARSENETWORK_H
#include <winsock2.h>

#include "Protocols/NetworkLayer.h"

namespace wcls {
    using namespace wcls::protocol;

    const u_char* ParseIpv4(const u_char* packet, IPv4Header* header, uint32_t& caplen);

    const u_char* ParseIpv6(const u_char* packet, IPv6Header* header, uint32_t& caplen);
}

#endif //PARSENETWORK_H
