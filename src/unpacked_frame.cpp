#include "unpacked_frame.h"

namespace network_counter {

std::ostream& operator<<(std::ostream& os, const UnpackedFrame& frame)
{
    os << "=== Frame ========================================\n";
    os << "======== Ethernet header:\n";
    os << "\tDestination MAC: " << frame.ethHdr.dstMac << '\n';
    os << "\tSource MAC: " << frame.ethHdr.srcMac << '\n';
    os << "\tProtocol: " << frame.ethHdr.type << '\n';

    os << "======== IPv4 header:\n";
    os << "\tDestination IP: " << frame.ipHdr.dstIP << '\n';
    os << "\tSource IP: " << frame.ipHdr.srcIP << '\n';
    os << "\tProtocol IP: " << frame.ipHdr.protocol << '\n';

    if (std::holds_alternative<TCP>(frame.ipPayload))
    {
        os << "======== TCP packet:\n";
        auto payload = std::get<TCP>(frame.ipPayload);
        os << "\tSource port: " << payload.srcPort << '\n';
        os << "\tDestination port: " << payload.dstPort << '\n';
        os << "\tHeader length: " << payload.length << '\n';
    }
    else if (std::holds_alternative<UDP>(frame.ipPayload))
    {
        os << "======== UDP packet:\n";
        auto payload = std::get<UDP>(frame.ipPayload);
        os << "\tDestination port: " << payload.dstPort << '\n';
        os << "\tSource port: " << payload.srcPort << '\n';
        os << "\tLength: " << payload.length << '\n';
    }
    else if (std::holds_alternative<ICMP>(frame.ipPayload))
    {
        os << "======== ICMP packet:\n";
        auto payload = std::get<ICMP>(frame.ipPayload);
        os << "\tType: " << payload.type << '\n';
        os << "\tCode: " << payload.code << '\n';
    }
    else if (std::holds_alternative<IGMP>(frame.ipPayload))
    {
        os << "======== IGMP packet:\n";
        auto payload = std::get<IGMP>(frame.ipPayload);
        os << "\tType: " << payload.type << '\n';
        os << "\tCode: " << payload.routingCode << '\n';
        os << "\tGroup address: " << payload.groupAddr << '\n';
    }

    return os << std::endl;;
}

/**
        frame.ethHdr.dstMac = std::string(tmpDstMac); // string
        frame.ethHdr.srcMac = std::string(tmpSrcMac); // string
        frame.ethHdr.type = static_cast<uint16_t>(ethType); // uint16_t
        frame.ipHdr.dstIP = std::string(tmpDstIP); // string
        frame.ipHdr.srcIP = std::string(tmpSrcIP); // string
        frame.ipHdr.protocol = static_cast<uint32_t>(ipProto); // uint32_t
 * */
size_t 
serializeIPpayload
(const std::variant<ICMP, IGMP, TCP, UDP>* ipPayload, unsigned char* bytes)
{
    size_t i = 0;
    
    if (std::holds_alternative<ICMP>(*ipPayload))
    {
        bytes[i] = static_cast<byte>(PayloadType::ICMP); ++i;
        
        auto icmpRef = std::get<ICMP>(*ipPayload); 
        //uint32
        bytes[i] =  static_cast<byte>(icmpRef.type >> 24); ++i;
        bytes[i] =  static_cast<byte>(icmpRef.type >> 16); ++i;
        bytes[i] =  static_cast<byte>(icmpRef.type >> 8); ++i;
        bytes[i] =  static_cast<byte>(icmpRef.type); ++i;
        //uint32
        bytes[i] =  static_cast<byte>(icmpRef.code >> 24); ++i;
        bytes[i] =  static_cast<byte>(icmpRef.code >> 16); ++i;
        bytes[i] =  static_cast<byte>(icmpRef.code >> 8); ++i;
        bytes[i] =  static_cast<byte>(icmpRef.code); ++i;
        
        return i;
    }
    else if (std::holds_alternative<IGMP>(*ipPayload))
    {
        bytes[i] = static_cast<byte>(PayloadType::IGMP); ++i;

        auto igmpRef = std::get<IGMP>(*ipPayload);
        //uint32
        bytes[i] =  static_cast<byte>(igmpRef.type >> 24); ++i;
        bytes[i] =  static_cast<byte>(igmpRef.type >> 16); ++i;
        bytes[i] =  static_cast<byte>(igmpRef.type >> 8); ++i;
        bytes[i] =  static_cast<byte>(igmpRef.type); ++i;

        //uint32
        bytes[i] =  static_cast<byte>(igmpRef.routingCode >> 24); ++i;
        bytes[i] =  static_cast<byte>(igmpRef.routingCode >> 16); ++i;
        bytes[i] =  static_cast<byte>(igmpRef.routingCode >> 8); ++i;
        bytes[i] =  static_cast<byte>(igmpRef.routingCode); ++i;
 
        size_t groupAddrStrLen = igmpRef.groupAddr.size();
        bytes[i] = static_cast<byte>(groupAddrStrLen); ++i;
        for (size_t j = 0; j < groupAddrStrLen; ++j, ++i)
        {
            bytes[i] = static_cast<byte>(igmpRef.groupAddr[j]);
        }

        return i;
    }
    else if (std::holds_alternative<UDP>(*ipPayload))
    {
        bytes[i] = static_cast<byte>(PayloadType::UDP); ++i;
      
        auto refUdp = std::get<UDP>(*ipPayload);
        //uint32
        bytes[i] = static_cast<byte>(refUdp.srcPort >> 24); ++i;
        bytes[i] = static_cast<byte>(refUdp.srcPort >> 16); ++i;
        bytes[i] = static_cast<byte>(refUdp.srcPort >> 8); ++i;
        bytes[i] = static_cast<byte>(refUdp.srcPort); ++i;

        //uint32
        bytes[i] = static_cast<byte>(refUdp.dstPort >> 24); ++i;
        bytes[i] = static_cast<byte>(refUdp.dstPort >> 16); ++i;
        bytes[i] = static_cast<byte>(refUdp.dstPort >> 8); ++i;
        bytes[i] = static_cast<byte>(refUdp.dstPort); ++i;

        //uint32
        bytes[i] = static_cast<byte>(refUdp.length >> 24); ++i;
        bytes[i] = static_cast<byte>(refUdp.length >> 16); ++i;
        bytes[i] = static_cast<byte>(refUdp.length >> 8); ++i;
        bytes[i] = static_cast<byte>(refUdp.length); ++i;

        return i;
    }
    else if (std::holds_alternative<TCP>(*ipPayload))
    {
        bytes[i] = static_cast<byte>(PayloadType::TCP); ++i;
      
        auto refUdp = std::get<TCP>(*ipPayload);
        //uint32
        bytes[i] = static_cast<byte>(refUdp.srcPort >> 24); ++i;
        bytes[i] = static_cast<byte>(refUdp.srcPort >> 16); ++i;
        bytes[i] = static_cast<byte>(refUdp.srcPort >> 8); ++i;
        bytes[i] = static_cast<byte>(refUdp.srcPort); ++i;

        //uint32
        bytes[i] = static_cast<byte>(refUdp.dstPort >> 24); ++i;
        bytes[i] = static_cast<byte>(refUdp.dstPort >> 16); ++i;
        bytes[i] = static_cast<byte>(refUdp.dstPort >> 8); ++i;
        bytes[i] = static_cast<byte>(refUdp.dstPort); ++i;

        //uint32
        bytes[i] = static_cast<byte>(refUdp.length >> 24); ++i;
        bytes[i] = static_cast<byte>(refUdp.length >> 16); ++i;
        bytes[i] = static_cast<byte>(refUdp.length >> 8); ++i;
        bytes[i] = static_cast<byte>(refUdp.length); ++i;

        return i;
    }
    return i;
}

std::variant<ICMP, IGMP, TCP, UDP>
deserializeIPpayload
(const unsigned char* bytes)
{
    std::variant<ICMP, IGMP, TCP, UDP> result;
    
    size_t i = 0;
    PayloadType type = static_cast<PayloadType>(bytes[i]); ++i;
    
    if (type == PayloadType::ICMP)
    {
        ICMP icmp;
        icmp.type = 0;
        icmp.type |= static_cast<uint32_t>(bytes[i]); ++i;
        icmp.type <<= 8;
        icmp.type |= static_cast<uint32_t>(bytes[i]); ++i;
        icmp.type <<= 8;
        icmp.type |= static_cast<uint32_t>(bytes[i]); ++i;
        icmp.type <<= 8;
        icmp.type |= static_cast<uint32_t>(bytes[i]); ++i;
        
        icmp.code = 0;
        icmp.code |= static_cast<uint32_t>(bytes[i]); ++i;
        icmp.code <<= 8;
        icmp.code |= static_cast<uint32_t>(bytes[i]); ++i;
        icmp.code <<= 8;
        icmp.code |= static_cast<uint32_t>(bytes[i]); ++i;
        icmp.code <<= 8;
        icmp.code |= static_cast<uint32_t>(bytes[i]); ++i;
        
        result = icmp; 
    }
    else if (type == PayloadType::IGMP)
    {
        IGMP igmp;
        igmp.type = 0;
        igmp.type |= static_cast<uint32_t>(bytes[i]); ++i;
        igmp.type <<= 8;
        igmp.type |= static_cast<uint32_t>(bytes[i]); ++i;
        igmp.type <<= 8;
        igmp.type |= static_cast<uint32_t>(bytes[i]); ++i;
        igmp.type <<= 8;
        igmp.type |= static_cast<uint32_t>(bytes[i]); ++i;
 
        igmp.routingCode = 0;
        igmp.routingCode |= static_cast<uint32_t>(bytes[i]); ++i;
        igmp.routingCode <<= 8;
        igmp.routingCode |= static_cast<uint32_t>(bytes[i]); ++i;
        igmp.routingCode <<= 8;
        igmp.routingCode |= static_cast<uint32_t>(bytes[i]); ++i;
        igmp.routingCode <<= 8;
        igmp.routingCode |= static_cast<uint32_t>(bytes[i]); ++i;
        
        size_t groupAddrLen = static_cast<size_t>(bytes[i]); ++i;
        for (size_t j = 0; j < groupAddrLen; ++j, ++i)
        {
            igmp.groupAddr += bytes[i];
        }

        result = igmp;
    }
    else if (type == PayloadType::TCP)
    {
        TCP tcp;
        
        tcp.srcPort = 0;
        tcp.srcPort |= static_cast<uint32_t>(bytes[i]); ++i;
        tcp.srcPort <<= 8;
        tcp.srcPort |= static_cast<uint32_t>(bytes[i]); ++i;
        tcp.srcPort <<= 8;
        tcp.srcPort |= static_cast<uint32_t>(bytes[i]); ++i;
        tcp.srcPort <<= 8;
        tcp.srcPort |= static_cast<uint32_t>(bytes[i]); ++i;

        tcp.dstPort = 0;
        tcp.dstPort |= static_cast<uint32_t>(bytes[i]); ++i;
        tcp.dstPort <<= 8;
        tcp.dstPort |= static_cast<uint32_t>(bytes[i]); ++i;
        tcp.dstPort <<= 8;
        tcp.dstPort |= static_cast<uint32_t>(bytes[i]); ++i;
        tcp.dstPort <<= 8;
        tcp.dstPort |= static_cast<uint32_t>(bytes[i]); ++i;

        tcp.length = 0;
        tcp.length |= static_cast<uint32_t>(bytes[i]); ++i;
        tcp.length <<= 8;
        tcp.length |= static_cast<uint32_t>(bytes[i]); ++i;
        tcp.length <<= 8;
        tcp.length |= static_cast<uint32_t>(bytes[i]); ++i;
        tcp.length <<= 8;
        tcp.length |= static_cast<uint32_t>(bytes[i]); ++i;
        
        result = tcp;
    }
    else if (type == PayloadType::UDP)
    {
        UDP udp;
        
        udp.srcPort = 0;
        udp.srcPort |= static_cast<uint32_t>(bytes[i]); ++i;
        udp.srcPort <<= 8;
        udp.srcPort |= static_cast<uint32_t>(bytes[i]); ++i;
        udp.srcPort <<= 8;
        udp.srcPort |= static_cast<uint32_t>(bytes[i]); ++i;
        udp.srcPort <<= 8;
        udp.srcPort |= static_cast<uint32_t>(bytes[i]); ++i;

        udp.dstPort = 0;
        udp.dstPort |= static_cast<uint32_t>(bytes[i]); ++i;
        udp.dstPort <<= 8;
        udp.dstPort |= static_cast<uint32_t>(bytes[i]); ++i;
        udp.dstPort <<= 8;
        udp.dstPort |= static_cast<uint32_t>(bytes[i]); ++i;
        udp.dstPort <<= 8;
        udp.dstPort |= static_cast<uint32_t>(bytes[i]); ++i;

        udp.length = 0;
        udp.length |= static_cast<uint32_t>(bytes[i]); ++i;
        udp.length <<= 8;
        udp.length |= static_cast<uint32_t>(bytes[i]); ++i;
        udp.length <<= 8;
        udp.length |= static_cast<uint32_t>(bytes[i]); ++i;
        udp.length <<= 8;
        udp.length |= static_cast<uint32_t>(bytes[i]); ++i;
        
        result = udp;
    }

    return result;
}




}

