//
// Created by Mgepahmge on 25-4-14.
//
#include "ParseEthernet.h"

const u_char* wcls::ParseEthernet(const u_char* packet, EthernetHeader* header, uint32_t caplen) {
    if (caplen < 14) return nullptr;

    memcpy(header->destMAC, packet, 6);
    memcpy(header->srcMAC, packet + 6, 6);
    header->etherType = *(uint16_t*)(packet + 12);

    if (header->etherType == htons(0x8100)) {
        if (caplen < 18) return nullptr;

        header->vlanInfo = *(uint16_t*)(packet + 14);
        header->vlanEtherType = *(uint16_t*)(packet + 16);

        return packet + 18;
    } else {
        header->vlanInfo = 0;
        header->vlanEtherType = 0;

        return packet + 14;
    }
}
