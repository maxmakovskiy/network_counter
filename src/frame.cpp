#include "frame.h"

namespace network_counter {

RawFrame::RawFrame
(unsigned char* raw, int rawLen)
    : rawLen(rawLen)
{
    ip = reinterpret_cast<struct iphdr*>(raw + sizeof(ethhdr));
    eth = reinterpret_cast<struct ethhdr*>(raw);
   
    if (eth->h_proto == 0x86dd) {
        ipVer = IPVer::IPv6;
    } else {
        ipVer = IPVer::IPv4;
    }

    iphdrLen = ip->ihl * 4;

    memset(&srcIn, 0, sizeof(srcIn));
    memset(&dstIn, 0, sizeof(dstIn));
    srcIn.sin_addr.s_addr = ip->saddr;
    dstIn.sin_addr.s_addr = ip->daddr;

#ifdef VISUAL_DEBUG
    std::cout << static_cast<unsigned int>(ip->protocol) << '\n';
#endif

    if (ip->protocol == 6) 
    {
        tcp = reinterpret_cast<struct tcphdr*>(raw + sizeof(ethhdr) + iphdrLen);
        data = raw + iphdrLen + sizeof(ethhdr) + sizeof(tcphdr);
        underlyingType = TransportType::TCP;
    }
    else if (ip->protocol == 17)
    {
        udp = reinterpret_cast<struct udphdr*>(raw + sizeof(ethhdr) + iphdrLen);
        data = raw + iphdrLen + sizeof(ethhdr) + sizeof(udphdr);
        underlyingType = TransportType::UDP;
    } 
    else if (ip->protocol == 1)
    {
        icmp = reinterpret_cast<struct icmphdr*>(raw + sizeof(ethhdr) + iphdrLen);
        data = raw + iphdrLen + sizeof(ethhdr) + sizeof(icmphdr);
        underlyingType = TransportType::ICMP;
    }
    else if (ip->protocol == 2)
    {
        igmp = reinterpret_cast<struct igmp*>(raw + sizeof(ethhdr) + iphdrLen);
        data = raw + iphdrLen + sizeof(ethhdr) + sizeof(igmp);
        underlyingType = TransportType::IGMP;
    }
    else
    {
        tcp = nullptr;
        udp = nullptr;
        icmp = nullptr;
        underlyingType = TransportType::OTHER;
    }

}

std::string
RawFrame::getMacStr
(const unsigned char* rawMac) const
{
    char tmpBuffer[18];
    sprintf(tmpBuffer, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", rawMac[0],
            rawMac[1], rawMac[2], rawMac[3], rawMac[4], rawMac[5]); 
    return std::string(tmpBuffer);
} 

std::string
RawFrame::getProtocolStr() const
{
    char tmpBuffer[10];
    sprintf(tmpBuffer, "0x%04x", ntohs(eth->h_proto));
    return std::string(tmpBuffer);
}

std::string
RawFrame::ip4GetIPStr
(const struct in_addr* addr) const
{
    char address[20];
    sprintf(address, "%s", inet_ntoa(*addr));
    return std::string(address);
}

uint32_t RawFrame::ip4GetIPProtocol() const
{
    return static_cast<uint32_t>(ip->protocol);
}

UnpackedFrame RawFrame::GetUnpackedFrame() const
{
    UnpackedFrame result;
    
    result.ethHdr.dstMac = getMacStr(eth->h_dest); //string
    result.ethHdr.srcMac = getMacStr(eth->h_source); //string
    result.ethHdr.type = ntohs(eth->h_proto); //uint16_t

    if (ipVer == IPVer::IPv4)
    {
        result.ipHdr.dstIP = ip4GetIPStr(&dstIn.sin_addr); //string
        result.ipHdr.srcIP = ip4GetIPStr(&srcIn.sin_addr); //string
        result.ipHdr.protocol = ip4GetIPProtocol(); //uint32_t
    }
    else if (ipVer == IPVer::IPv6)
    {
/*
        result.ipHdr.dstIP = ip6GetDstIPAddrStr(); //string
        result.ipHdr.srcIP = ip6GetSrcIPAddrStr(); //string
        result.ipHdr.protocol = ip6GetIPProtocol(); //uint32_t
*/
    }
    
    // choose ICMP vs IGMP vs TCP vs UDP
    switch (underlyingType) 
    {
    case TransportType::TCP: {
       result.ipPayload = TCP{
             ntohs(tcp->source),
             ntohs(tcp->dest),
             static_cast<unsigned int>(tcp->doff)*4 
        };
    } break;
    case TransportType::UDP: {
       result.ipPayload = UDP{
             ntohs(udp->source),
             ntohs(udp->dest),
             ntohs(udp->len)
        };
    } break;
    case TransportType::ICMP: {
        result.ipPayload = ICMP{
            icmp->type,
            icmp->code
        };
    } break;
    case TransportType::IGMP: {
        result.ipPayload = IGMP{
            igmp->igmp_type,
            igmp->igmp_code,
            ip4GetIPStr(&igmp->igmp_group)
        };
    } break;
    default:
        // nothing to show
        break;
    }

    return result;
}

}

