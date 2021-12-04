#include "frame.h"

namespace network_counter {

RawFrame::RawFrame
(unsigned char* raw, int rawLen)
    : rawLen(rawLen)
{
    ip = reinterpret_cast<struct iphdr*>(raw + sizeof(ethhdr));
    eth = reinterpret_cast<struct ethhdr*>(raw);
   
    if (GetProtocol() == 0x86dd) {
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
    {// TCP     
        tcp = reinterpret_cast<struct tcphdr*>(raw + sizeof(ethhdr) + iphdrLen);
        data = raw + iphdrLen + sizeof(ethhdr) + sizeof(tcphdr);
        underlyingType = ProtocolType::TCP;
    }
    else if (ip->protocol == 17)
    {// UDP
        udp = reinterpret_cast<struct udphdr*>(raw + sizeof(ethhdr) + iphdrLen);
        data = raw + iphdrLen + sizeof(ethhdr) + sizeof(udphdr);
        underlyingType = ProtocolType::UDP;
    } 
    else if (ip->protocol == 1)
    {
        icmp = reinterpret_cast<struct icmphdr*>(raw + sizeof(ethhdr) + iphdrLen);
        data = raw + iphdrLen + sizeof(ethhdr) + sizeof(icmphdr);
        underlyingType = ProtocolType::ICMP;
    }
    else if (ip->protocol == 2)
    {
        igmp = reinterpret_cast<struct igmp*>(raw + sizeof(ethhdr) + iphdrLen);
        data = raw + iphdrLen + sizeof(ethhdr) + sizeof(igmp);
        underlyingType = ProtocolType::IGMP;
    }
    else
    {
        tcp = nullptr;
        udp = nullptr;
        icmp = nullptr;
        underlyingType = ProtocolType::OTHER;
    }

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
    return ntohs(eth->h_proto);
}

std::string
RawFrame::GetProtocolStr() const
{
    char res[10];
    sprintf(res, "0x%04x", GetProtocol());

    return std::string(res);
}

std::string 
RawFrame::GetSrcIPAddrStr() const
{
    return getIPStr(&srcIn.sin_addr);
}

std::string
RawFrame::GetDstIPAddrStr() const
{
    return getIPStr(&dstIn.sin_addr);
}

std::string
RawFrame::getIPStr
(const struct in_addr* addr) const
{
    char address[20];
    sprintf(address, "%s", inet_ntoa(*addr));
    return std::string(address);
}

ProtocolType
RawFrame::GetProtoType() const
{
    return underlyingType;
}

IcmpRepresentation
RawFrame::GetIcmpRepresentation() const
{
    return IcmpRepresentation{
            icmp->type,
            icmp->code };
}

TcpRepresentation
RawFrame::GetTcpRepresentation() const
{
    return TcpRepresentation{ 
             ntohs(tcp->source),
             ntohs(tcp->dest),
             ntohl(tcp->seq),
             ntohl(tcp->ack_seq),
             static_cast<unsigned int>(tcp->doff)*4 };
}

UdpRepresentation
RawFrame::GetUdpRepresentation() const
{
    return UdpRepresentation{
             ntohs(udp->source),
             ntohs(udp->dest),
             ntohs(udp->len) };
}

IgmpRepresentation
RawFrame::GetIgmpRepresentation() const
{
    char res[10];
    sprintf(res, "0x%02x", igmp->igmp_type);

    return IgmpRepresentation{
            std::string(res),
            igmp->igmp_code,
            getIPStr(&igmp->igmp_group) };
}

IPVer
RawFrame::GetIPVersion() const
{
    return ipVer;
}

std::ostream& operator<<(std::ostream& os, const RawFrame& frame)
{
    os << "=== Frame ========================================\n";
    os << "======== Ethernet header:\n";
    os << "\tDestination MAC: " << frame.GetDstMacStr() << '\n';
    os << "\tSource MAC: " << frame.GetSrcMacStr() << '\n';
    os << "\tProtocol: " << frame.GetProtocolStr() << '\n';

    if (frame.GetIPVersion() == IPVer::IPv4) 
    {
        os << "======== IPv4 header:\n";
        os << "\tDestination IP: " << frame.GetDstIPAddrStr() << '\n';
        os << "\tSource IP: " << frame.GetSrcIPAddrStr() << '\n';
    }
    else
    {
        os << "======== IPv6 header:\n";
    }

    switch (frame.GetProtoType()) 
    {
    case ProtocolType::TCP: {
        os << "======== TCP packet:\n";
        auto represent = frame.GetTcpRepresentation();
        os << "\tSource port: " << represent.srcPort << '\n';
        os << "\tDestination port: " << represent.dstPort << '\n';
        os << "\tSequence number: " << represent.seqNumber << '\n';
        os << "\tAcknowledge number: " << represent.ackNumber << '\n';
        os << "\tHeader length: " << represent.headerLength << '\n';
        } break;
    case ProtocolType::UDP: {
        os << "======== UDP packet:\n";
        auto represent = frame.GetUdpRepresentation();
        os << "\tDestination port: " << represent.dstPort << '\n';
        os << "\tSource port: " << represent.srcPort << '\n';
        os << "\tLength: " << represent.length << '\n';
        } break;
    case ProtocolType::ICMP: {
        os << "======== ICMP packet:\n";
        auto represent = frame.GetIcmpRepresentation();
        os << "\tType: " << represent.type << '\n';
        os << "\tCode: " << represent.code << '\n';
        } break;
    case ProtocolType::IGMP: {
        os << "======== IGMP packet:\n";
        auto represent = frame.GetIgmpRepresentation();
        os << "\tType: " << represent.type << '\n';
        os << "\tCode: " << represent.routingCode << '\n';
        os << "\tGroup address: " << represent.groupAddr << '\n';
        } break;
    default:
        // nothing to show
        break;
    }

    return os << std::endl;;
}

}

