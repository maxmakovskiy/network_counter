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

}

