#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>
#include <stdio.h>
#include <linux/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <netinet/igmp.h>
#include <arpa/inet.h>

namespace network_counter { 

// TODO:
// - Add support for IPv6 -> eth->h_proto == 0x86DD
// - Add writing output to file

enum class ProtocolType
{
    TCP, UDP, ICMP, IGMP, OTHER
};

enum class IPVer
{
    IPv4, IPv6
};

struct TcpRepresentation
{
    unsigned int srcPort;
    unsigned int dstPort;
    unsigned int seqNumber; 
    unsigned int ackNumber;
    unsigned int headerLength;
};

struct UdpRepresentation
{
    unsigned int srcPort;
    unsigned int dstPort;
    unsigned int length;
};

struct IcmpRepresentation
{
    unsigned int type;
    unsigned int code;
};

struct IgmpRepresentation
{
    std::string type;
    unsigned int routingCode;
    std::string groupAddr;
};

class RawFrame
{
public:
    explicit RawFrame(unsigned char* raw, int rawLen);
    
    std::string GetSrcMacStr() const;
    std::string GetDstMacStr() const;
    uint16_t GetProtocol() const;
    std::string GetProtocolStr() const;

    std::string GetSrcIPAddrStr() const;
    std::string GetDstIPAddrStr() const;
    IPVer GetIPVersion() const;

    IcmpRepresentation GetIcmpRepresentation() const;
    TcpRepresentation GetTcpRepresentation() const;
    UdpRepresentation GetUdpRepresentation() const;
    IgmpRepresentation GetIgmpRepresentation() const;

    ProtocolType GetProtoType() const;

private:
    unsigned char* data; 
    int rawLen;
    int iphdrLen;
    ProtocolType underlyingType;
    IPVer ipVer;

    // raw mappings from unix headers 
    struct iphdr* ip;
    struct ethhdr* eth;
    struct sockaddr_in srcIn, dstIn;
    struct tcphdr* tcp;
    struct udphdr* udp;
    struct icmphdr* icmp;
    struct igmp* igmp;
    
    std::string getMacStr(const unsigned char* rawMac) const;
    std::string getIPStr(const struct in_addr* addr) const;

};

std::ostream& operator<<(std::ostream& os, const RawFrame& frame);

}

