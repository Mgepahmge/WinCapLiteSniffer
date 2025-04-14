//
// Created by Mgepahmge on 25-4-13.
//

#ifndef DATALINKLAYER_H
#define DATALINKLAYER_H
#include <cstdint>

namespace wcls::protocol  {

#pragma pack(push, 1)
    struct EthernetHeader {
        uint8_t  destMAC[6];
        uint8_t  srcMAC[6];
        uint16_t etherType;
        uint16_t vlanInfo;
        uint16_t vlanEtherType;
    };
#pragma pack(pop)


}

#endif //DATALINKLAYER_H
