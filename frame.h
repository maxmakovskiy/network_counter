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
#include <arpa/inet.h>

namespace network_counter { 

enum class ProtocolType
{
    TCP, UDP, ICMP, OTHER
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

class RawFrame
{
public:
    explicit RawFrame(unsigned char* raw, int rawLen);
    
    std::string GetSrcMacStr() const;
    std::string GetDstMacStr() const;
    uint16_t GetProtocol() const;

    std::string GetSrcIPAddrStr() const;
    std::string GetDstIPAddrStr() const;

    IcmpRepresentation GetIcmpRepresentation() const;
    TcpRepresentation GetTcpRepresentation() const;
    UdpRepresentation GetUdpRepresentation() const;

    ProtocolType GetProtoType() const;

private:
    unsigned char* data; 
    int rawLen;
    int iphdrLen;
    ProtocolType underlyingType;

    // raw mappings from unix headers 
    struct iphdr* ip;
    struct ethhdr* eth;
    struct sockaddr_in srcIn, dstIn;
    struct tcphdr* tcp;
    struct udphdr* udp;
    struct icmphdr* icmp;
    
    std::string getMacStr(const unsigned char* rawMac) const;
    std::string getIPStr(const struct in_addr* addr) const;

};

std::ostream& operator<<(std::ostream& os, const RawFrame& frame);

}

