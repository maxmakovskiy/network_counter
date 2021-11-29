#pragma once

#include <cstddef>
#include <vector>
#include <array>
#include <string>
#include <netinet/in.h>
#include <cstdint>

namespace network_counter { 

class EthernetFrame
{
public:
    explicit EthernetFrame(const std::vector<std::byte>& raw);
    
    std::string GetSrcMacStr() const;
    std::string GetDstMacStr() const;
    uint16_t GetPrototype() const;
    
private:
    std::array<std::byte, 6> srcMac;
    std::array<std::byte, 6> dstMac;
    uint16_t prototype;
    std::vector<std::byte> data;
    
    std::string getMacStr(const std::array<std::byte, 6>& rawMac) const;

};

}

