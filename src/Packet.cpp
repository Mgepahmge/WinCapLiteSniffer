//
// Created by Mgepahmge on 25-4-14.
//
#include "Packet.h"

wcls::Packet::Packet(const u_char* data, const struct pcap_pkthdr* pkthdr) : data(data), pkthdr(pkthdr), ethernetHeader(),
                                                                 ipv4Header(), ipv6Header(), tcpHeader(), udpHeader(),
                                                                 ethernetEnable(false), ipv4Enable(false),
                                                                 ipv6Enable(false), tcpEnable(false),
                                                                 udpEnable(false) {
    uint32_t caplen = pkthdr->len;
    const u_char* temp = data;
    temp = ParseEthernet(temp, &ethernetHeader, caplen);
    if (temp == nullptr) {
        return;
    }
    ethernetEnable = true;
    if (ethernetHeader.vlanInfo ? ethernetHeader.vlanEtherType : ethernetHeader.etherType == MY_ETHERTYPE_IP) {
        temp = ParseIpv4(temp, &ipv4Header, caplen);
        if (temp == nullptr) {
            return;
        }
        ipv4Enable = true;
        switch (ipv4Header.protocol) {
            case MY_IPPROTO_TCP: {
                tcpEnable = ParseTCP(temp, &tcpHeader, caplen);
                break;
            }
            case MY_IPPROTO_UDP: {
                udpEnable = ParseUDP(temp, &udpHeader, caplen);
                break;
            }
            default:
                break;
        }
    } else if (ethernetHeader.vlanInfo ? ethernetHeader.vlanEtherType : ethernetHeader.etherType == MY_ETHERTYPE_IPV6) {
        temp = ParseIpv6(temp, &ipv6Header, caplen);
        if (temp == nullptr) {
            return;
        }
        ipv6Enable = true;
        switch (ipv6Header.next_header) {
            case MY_IPPROTO_TCP: {
                tcpEnable = ParseTCP(temp, &tcpHeader, caplen);
                break;
            }
            case MY_IPPROTO_UDP: {
                udpEnable = ParseUDP(temp, &udpHeader, caplen);
                break;
            }
            default:
                break;
        }
    }
}

wcls::Packet::~Packet() = default;

static std::string wcls::mac_to_string(const uint8_t mac[6]) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 6; ++i) {
        ss << std::setw(2) << static_cast<int>(mac[i]);
        if (i != 5) ss << ":";
    }
    return ss.str();
}

inline std::ostream& wcls::operator<<(std::ostream& os, const wcls::protocol::EthernetHeader& header) {
    os << "Ethernet [";
    os << "Dest: " << wcls::mac_to_string(header.destMAC)
       << "  Src: " << wcls::mac_to_string(header.srcMAC);

    if (ntohs(header.etherType) == 0x8100 ||
        ntohs(header.etherType) == 0x88A8) {
        os << "  VLAN ["
           << "Type: 0x" << std::hex << std::setw(4)
           << ntohs(header.vlanEtherType) << " "
           << ETHER_TYPE_TO_STR(header.vlanEtherType)
           << " | PCP: " << std::dec << ((ntohs(header.vlanInfo) >> 13) & 0x07)
           << " DEI: " << ((ntohs(header.vlanInfo) >> 12) & 0x01)
           << " VID: " << (ntohs(header.vlanInfo) & 0x0FFF) << "]";
        } else {
            os << "  EtherType: 0x" << std::hex << std::setw(4)
               << ntohs(header.etherType) << " "
               << ETHER_TYPE_TO_STR(header.etherType);
        }
    os << "]";
    return os;
}

std::ostream& wcls::operator<<(std::ostream& os, const wcls::protocol::IPv4Header& header) {
    // 解析版本和头长度
    uint8_t version = (header.version_ihl >> 4) & 0x0F;
    uint8_t ihl = (header.version_ihl & 0x0F) * 4;
    // 转换网络字节序字段
    uint16_t total_len = ntohs(header.total_length);
    uint16_t flags_frag = ntohs(header.flags_fragment);

    // 提取分片信息
    bool df_flag = (flags_frag & 0x4000) != 0;
    bool mf_flag = (flags_frag & 0x2000) != 0;
    uint16_t frag_offset = flags_frag & 0x1FFF;
    // IP地址转换
    char src_ip[INET_ADDRSTRLEN], dst_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &header.srcIP, src_ip, sizeof(src_ip));
    inet_ntop(AF_INET, &header.destIP, dst_ip, sizeof(dst_ip));
    // 格式化输出
    os << "IPv4 ["
       << "v" << static_cast<int>(version)
       << " hlen=" << static_cast<int>(ihl)
       << " tos=0x" << std::hex << static_cast<int>(header.tos)
       << " len=" << std::dec << total_len
       << " id=0x" << std::hex << ntohs(header.identification)
       << " flags=" << (df_flag ? "DF " : "") << (mf_flag ? "MF " : "")
       << "offset=" << std::dec << frag_offset
       << " ttl=" << static_cast<int>(header.ttl)
       << " proto=" << IP_PROTOCOL_TO_STR(header.protocol)
       << " chksum=0x" << std::hex << ntohs(header.checksum)
       << " src=" << src_ip
       << " dst=" << dst_ip
       << "]";
    return os;
}

std::ostream& wcls::operator<<(std::ostream& os, const wcls::protocol::IPv6Header& header) {
    // 转换网络字节序
    uint32_t ver_class_flow = ntohl(header.version_class_flow);
    uint16_t payload_len = ntohs(header.payload_length);
    // 分解版本/流量类/流标签
    uint8_t version = (ver_class_flow >> 28) & 0x0F;
    uint8_t traffic_class = (ver_class_flow >> 20) & 0xFF;
    uint32_t flow_label = ver_class_flow & 0x000FFFFF;
    // IPv6地址转换
    char src_ip[INET6_ADDRSTRLEN], dst_ip[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, header.srcIP, src_ip, sizeof(src_ip));
    inet_ntop(AF_INET6, header.destIP, dst_ip, sizeof(dst_ip));
    // 格式化输出
    os << "IPv6 ["
       << "v" << static_cast<int>(version)
       << " tc=0x" << std::hex << static_cast<int>(traffic_class)
       << " flow=0x" << std::setw(5) << std::setfill('0') << flow_label
       << " plen=" << std::dec << payload_len
       << " next=" << IP_PROTOCOL_TO_STR(header.next_header)
       << " hlim=" << static_cast<int>(header.hop_limit)
       << " src=" << src_ip
       << " dst=" << dst_ip
       << "]";
    return os;
}

std::ostream& wcls::operator<<(std::ostream& os, const wcls::protocol::TCPHeader& header) {
    // 转换网络字节序
    uint16_t src_port = ntohs(header.src_port);
    uint16_t dst_port = ntohs(header.dest_port);
    uint32_t seq_num = ntohl(header.seq_num);
    uint32_t ack_num = ntohl(header.ack_num);
    uint16_t flags = ntohs(header.flags_doff);

    // 解析数据偏移和标志位
    uint8_t data_offset = (flags >> 12) & 0x0F; // 前4位
    uint8_t flags_bits = flags & 0x1FF;         // 低9位
    // 分解标志位
    bool urg = flags_bits & 0x20;  // URG
    bool ack = flags_bits & 0x10;  // ACK
    bool psh = flags_bits & 0x08;  // PSH
    bool rst = flags_bits & 0x04;  // RST
    bool syn = flags_bits & 0x02;  // SYN
    bool fin = flags_bits & 0x01;  // FIN
    // 格式化输出
    os << "TCP ["
       << "sport=" << src_port
       << " dport=" << dst_port
       << " seq=0x" << std::hex << seq_num
       << " ack=0x" << ack_num
       << " flags=";

    // 标志位字符串
    if (urg) os << "URG ";
    if (ack) os << "ACK ";
    if (psh) os << "PSH ";
    if (rst) os << "RST ";
    if (syn) os << "SYN ";
    if (fin) os << "FIN ";

    os << "win=" << std::dec << ntohs(header.window_size)
       << " cksum=0x" << std::hex << ntohs(header.checksum)
       << " urp=" << std::dec << ntohs(header.urgent_ptr)
       << " doff=" << static_cast<int>(data_offset) * 4 << "]";
    return os;
}

std::ostream& wcls::operator<<(std::ostream& os, const wcls::protocol::UDPHeader& header) {
    os << "UDP ["
       << "sport=" << ntohs(header.src_port)
       << " dport=" << ntohs(header.dest_port)
       << " len=" << ntohs(header.length)
       << " cksum=0x" << std::hex << ntohs(header.checksum) << "]";
    return os;
}

std::ostream& wcls::operator<<(std::ostream& os, const Packet& packet) {
    os << "Packet [" << std::endl;
    os << "Size: " << packet.pkthdr->len  << " bytes" << std::endl;
    if (packet.ethernetEnable) {
        os << packet.ethernetHeader << std::endl;
    }
    if (packet.ipv4Enable) {
        os << packet.ipv4Header << std::endl;
    }
    if (packet.ipv6Enable) {
        os << packet.ipv6Header << std::endl;
    }
    if (packet.tcpEnable) {
        os << packet.tcpHeader << std::endl;
    }
    if (packet.udpEnable) {
        os << packet.udpHeader << std::endl;
    }
    os << "]";
    return os;
}
