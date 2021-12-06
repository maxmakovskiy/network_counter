#pragma once

#include <iostream>
#include <string>
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

#include "unpacked_frame.h"

namespace network_counter { 

enum class TransportType
{
    TCP, UDP, ICMP, IGMP, OTHER
};

enum class IPVer
{
    IPv4, IPv6
};

class RawFrame
{
public:
    explicit RawFrame(unsigned char* raw, int rawLen);
    
    UnpackedFrame GetUnpackedFrame() const;

private:
    unsigned char* data; 
    int rawLen;
    int iphdrLen;
    TransportType underlyingType;
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
    std::string ip4GetIPStr(const struct in_addr* addr) const;
    std::string getProtocolStr() const;
    uint32_t ip4GetIPProtocol() const;

};

}

