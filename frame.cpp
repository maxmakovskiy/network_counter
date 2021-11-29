#include "frame.h"

namespace network_counter {

EthernetFrame::EthernetFrame
(unsigned char* raw)
{
    for (size_t i = 0; i < 15; i++) 
    {
        if (i >= 0 && i < 6) {
            srcMac[i] = std::byte{raw[i]};
        } else if (i >= 6 && i < 12) {
            dstMac[i] = std::byte{raw[i]};
        } else if (i >= 12 && i < 14) {
            if (i == 12) {
                protocol = static_cast<uint16_t>(raw[i]);
            } else {
                protocol = (protocol << 8) | static_cast<uint16_t>(raw[i]);
            }
        } else {
            data = raw+i;        
        }
    }

}


std::string
EthernetFrame::GetSrcMacStr() const
{
    return getMacStr(srcMac);
}

std::string
EthernetFrame::GetDstMacStr() const
{
    return getMacStr(dstMac);
}

std::string
EthernetFrame::getMacStr
(const std::array<std::byte, 6>& rawMac) const
{
    std::stringstream ss;

    ss << std::hex << std::to_integer<int>(rawMac[0]);
    ss << std::hex << std::to_integer<int>(rawMac[1]);
    ss << ":"; 
    ss << std::hex << std::to_integer<int>(rawMac[2]);
    ss << std::hex << std::to_integer<int>(rawMac[3]);
    ss << ":"; 
    ss << std::hex << std::to_integer<int>(rawMac[4]);
    ss << std::hex << std::to_integer<int>(rawMac[5]);
   
    return ss.str();
} 

uint16_t
EthernetFrame::GetProtocol() const
{
    return htons(protocol);
}

}

