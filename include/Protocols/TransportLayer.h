//
// Created by Mgepahmge on 25-4-13.
//

#ifndef TRANSPORTLAYER_H
#define TRANSPORTLAYER_H

namespace wcls::protocol  {

#pragma pack(push, 1)
    struct TCPHeader {
        uint16_t src_port;       // 源端口（网络字节序）
        uint16_t dest_port;      // 目标端口（网络字节序）
        uint32_t seq_num;        // 序列号
        uint32_t ack_num;        // 确认号

        // 数据偏移(4bit) + 保留(4bit) + 标志位(8bit)
        uint16_t flags_doff;     // 数据偏移量（单位4字节）| 标志位

        uint16_t window_size;    // 窗口大小
        uint16_t checksum;       // 校验和
        uint16_t urgent_ptr;     // 紧急指针
        // 可变长度选项字段（如果有）
    };

    struct UDPHeader {
        uint16_t src_port;       // 源端口
        uint16_t dest_port;      // 目标端口
        uint16_t length;         // 数据报长度（头+数据）
        uint16_t checksum;       // 校验和
    };
#pragma pack(pop)

}

#endif //TRANSPORTLAYER_H
