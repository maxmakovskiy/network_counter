#include "frame.h"

namespace network_counter {

EthernetFrame::EthernetFrame
(const std::vector<std::byte>& raw)
{
    data.reserve(raw.size() - 12);

    for (size_t i = 0; i < raw.size(); i++) 
    {
        if (i >= 0 && i < 6) {
            srcMac[i] = raw[i];
        } else if (i >= 6 && i < 11) {
            dstMac[i] = raw[i];
        } else if (i >= 12 && i < 14) {
            if (i == 12) {
                protocol = static_cast<uint16_t>(raw[i]);
            } else {
                protocol = (protocol << 8) | static_cast<uint16_t>(raw[i]);
            }
        } else {
            data.push_back(raw[i]);        
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
    std::string result;

    int i = 0;
    for (const std::byte& b : rawMac) 
    {
        if (i == 2) 
        {
            result += ':';
            i = 0;
        }

        i++;
        result += std::to_integer<int>(b);
    }
    
    return result;
} 

uint16_t
EthernetFrame::GetProtocol() const
{
    return htons(protocol);
}

}

