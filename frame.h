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
#include <arpa/inet.h>

namespace network_counter { 

class RawFrame
{
public:
    explicit RawFrame(unsigned char* raw, int rawLen);
    
    std::string GetSrcMacStr() const;
    std::string GetDstMacStr() const;
    uint16_t GetProtocol() const;

    std::string GetSrcIPAddrStr() const;
    std::string GetDstIPAddrStr() const;

private:
    unsigned char* data; 
    int rawLen;
    int iphdrLen;

    // raw mappings from unix headers 
    struct iphdr* ip;
    struct ethhdr* eth;
    struct sockaddr_in srcIn, dstIn;
    struct tcphdr* tcp;
    struct udphdr* udp;
    
    std::string getMacStr(const unsigned char* rawMac) const;
    std::string getIPStr(const struct in_addr* addr) const;

};

std::ostream& operator<<(std::ostream& os, const RawFrame& frame);

}

