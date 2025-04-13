//
// Created by Mgepahmge on 25-4-13.
//

#ifndef DATALINKLAYER_H
#define DATALINKLAYER_H
#include <cstdint>

namespace wcls::protocol  {

#pragma pack(push, 1)
    struct EthernetHeader {
        uint8_t  destMAC[6];    // 目标MAC地址 (6字节)
        uint8_t  srcMAC[6];     // 源MAC地址 (6字节)
        uint16_t etherType;      // 上层协议类型 (2字节)
        // 0x0800=IPv4, 0x86DD=IPv6
    };
#pragma pack(pop)

}

#endif //DATALINKLAYER_H
