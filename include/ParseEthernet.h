//
// Created by Mgepahmge on 25-4-14.
//

#ifndef PARSEETHERNET_H
#define PARSEETHERNET_H
#include <winsock2.h>

#include "Protocols/DataLinkLayer.h"

namespace wcls {
    using namespace protocol;
    const u_char* ParseEthernet(const u_char* packet, const EthernetHeader* header, bool& is_ipv4, uint32_t caplen);
}


#endif //PARSEETHERNET_H
