#ifndef MACRO_H
#define MACRO_H
#include <string>

#define MY_ETHERTYPE_IP        0x0008  // ntohs(0x0008)=0x0800
#define MY_ETHERTYPE_IPV6      0xDD86  // ntohs(0xDD86)=0x86DD
#define MY_ETHERTYPE_ARP       0x0608  // ntohs(0x0608)=0x0806
#define MY_ETHERTYPE_REVARP    0x3580  // ntohs(0x3580)=0x8035
#define MY_ETHERTYPE_VLAN      0x0081  // ntohs(0x0081)=0x8100
#define MY_ETHERTYPE_LOOPBACK  0x0080  // ntohs(0x0080)=0x8000
#define MY_ETHERTYPE_LLDP      0xCC88  // ntohs(0xCC88)=0x88CC
#define MY_ETHERTYPE_X25       0x0508  // ntohs(0x0508)=0x0805
#define MY_ETHERTYPE_MPLS      0x4788  // ntohs(0x4788)=0x8847

#define MY_IPPROTO_TCP      6
#define MY_IPPROTO_UDP      17
#define MY_IPPROTO_ICMP     1
#define MY_IPPROTO_IGMP     2
#define MY_IPPROTO_OSPF     89
#define MY_IPPROTO_SCTP     132
#define MY_IPPROTO_ICMPV6   58

#define ETHER_TYPE_TO_STR(etherType) ([&](){           \
    uint16_t _type = ntohs(etherType);                \
    if (_type == 0x0800)  return "IPv4";              \
    if (_type == 0x86DD)  return "IPv6";              \
    if (_type == 0x0806)  return "ARP";               \
    if (_type == 0x8035)  return "RARP";              \
    if (_type == 0x8100)  return "802.1Q VLAN";       \
    if (_type == 0x8000)  return "Loopback";          \
    if (_type == 0x88CC)  return "LLDP";              \
    if (_type == 0x0805)  return "X.25 over DIX";     \
    if (_type == 0x8847)  return "MPLS";              \
    return "Unknown";                                 \
})()

#define IP_PROTOCOL_TO_STR(proto) ([&](){              \
    switch(proto) {                                   \
        case 6:   return "TCP";                       \
        case 17:  return "UDP";                       \
        case 1:   return "ICMP";                     \
        case 2:   return "IGMP";                     \
        case 89:  return "OSPF";                     \
        case 132: return "SCTP";                     \
        case 58:  return "ICMPv6";                   \
        default:  return "Unknown";                  \
    }                                                 \
})()



#endif //MACRO_H
