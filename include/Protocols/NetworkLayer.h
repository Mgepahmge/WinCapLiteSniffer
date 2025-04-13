//
// Created by Mgepahmge on 25-4-13.
//

#ifndef NETWORKLAYER_H
#define NETWORKLAYER_H
#include <cstdint>

namespace wcls::protocol  {

#pragma pack(push, 1)
    struct IPv4Header {
        // 版本(4bit) + 头部长度(4bit, 单位4字节)
        uint8_t  version_ihl;    // 例如0x45表示版本4，头部长度5*4=20字节

        uint8_t  tos;            // 服务类型 (Type of Service)
        uint16_t total_length;   // 总长度（包含IP头+数据）
        uint16_t identification; // 分片标识符

        // 标志位(3bit) + 分片偏移(13bit)
        uint16_t flags_fragment; // 第1位:保留 | 第2位:DF | 第3位:MF | 后13位:分片偏移

        uint8_t  ttl;            // 生存时间
        uint8_t  protocol;       // 上层协议 6=TCP,17=UDP
        uint16_t checksum;       // 头部校验和
        uint32_t srcIP;          // 源IP地址（网络字节序）
        uint32_t destIP;         // 目标IP地址（网络字节序）
        // 可变长度选项字段（如果有）
    };
#pragma pack(pop)

}

#endif //NETWORKLAYER_H
