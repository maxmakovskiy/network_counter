#pragma once

#include <cstddef>
#include <vector>
#include <array>
#include <string>
#include <netinet/in.h>
#include <cstdint>
#include <sstream>

namespace network_counter { 

class EthernetFrame
{
public:
    explicit EthernetFrame(unsigned char* raw);
    
    std::string GetSrcMacStr() const;
    std::string GetDstMacStr() const;
    uint16_t GetProtocol() const;
    
private:
    std::array<std::byte, 6> srcMac;
    std::array<std::byte, 6> dstMac;
    uint16_t protocol;
    unsigned char* data;
    
    std::string getMacStr(const std::array<std::byte, 6>& rawMac) const;

};


}

