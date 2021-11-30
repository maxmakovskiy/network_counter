#include "frame.h"

namespace network_counter {

RawFrame::RawFrame
(unsigned char* raw, int rawLen)
    : rawLen(rawLen)
{
    ip = reinterpret_cast<struct iphdr*>(raw + sizeof(ethhdr));
    eth = reinterpret_cast<struct ethhdr*>(raw);
    
    iphdrLen = ip->ihl * 4;

    memset(&srcIn, 0, sizeof(srcIn));
    memset(&dstIn, 0, sizeof(dstIn));
    srcIn.sin_addr.s_addr = ip->saddr;
    dstIn.sin_addr.s_addr = ip->daddr;

    if (ip->protocol == 6) 
    {// TCP     
        tcp = reinterpret_cast<struct tcphdr*>(raw + sizeof(ethhdr) + iphdrLen);
        data = raw + iphdrLen + sizeof(ethhdr) + sizeof(tcphdr);
    }
    else if (ip->protocol == 17)
    {// UDP
        udp = reinterpret_cast<struct udphdr*>(raw + sizeof(ethhdr) + iphdrLen);
        data = raw + iphdrLen + sizeof(ethhdr) + sizeof(udphdr);
    }

/*
    protocol = static_cast<uint16_t>(raw[i]);
    protocol = (protocol << 8) | static_cast<uint16_t>(raw[i]);
*/
}


std::string
RawFrame::GetSrcMacStr() const
{
    return getMacStr(eth->h_source);
}

std::string
RawFrame::GetDstMacStr() const
{
    return getMacStr(eth->h_dest);
}

std::string
RawFrame::getMacStr
(const unsigned char* rawMac) const
{
    char result[18];
    
    sprintf(result, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", rawMac[0],
            rawMac[1], rawMac[2], rawMac[3], rawMac[4], rawMac[5]); 
       
    return std::string(result);
} 

uint16_t
RawFrame::GetProtocol() const
{
    return eth->h_proto;
}


std::ostream& operator<<(std::ostream& os, const RawFrame& frame)
{
    os << "=== Frame ========================================\n";
    os << "== Ethernet header:\n";
    os << "\tDestination: " << frame.GetDstMacStr() << '\n';
    os << "\tSource: " << frame.GetSrcMacStr() << '\n';
    os << "\tProtocol: " << frame.GetProtocol() << '\n';

    return os;
}

}

