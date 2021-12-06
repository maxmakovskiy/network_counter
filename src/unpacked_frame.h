#pragma once

#include <string>
#include <cstdint>
#include <variant>
#include <iostream>

namespace network_counter {

struct EthHeader
{
    std::string dstMac;
    std::string srcMac;
    uint16_t type;
};

struct IPHeader
{
    std::string dstIP;
    std::string srcIP;
    uint32_t protocol;
};

struct ICMP 
{
    uint32_t type;
    uint32_t code;
};

struct IGMP
{
    uint32_t type;
    uint32_t routingCode;
    std::string groupAddr;
};

struct TCP
{
    uint32_t srcPort;  
    uint32_t dstPort;  
    uint32_t length;  
};

struct UDP
{
    uint32_t srcPort;  
    uint32_t dstPort;  
    uint32_t length;  
};

struct UnpackedFrame
{
    EthHeader ethHdr;
    IPHeader ipHdr;
    std::variant<ICMP, IGMP, TCP, UDP> ipPayload;
};

std::ostream& operator<<(std::ostream& os, const UnpackedFrame& frame);

}


