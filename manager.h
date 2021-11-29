#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <array>
#include <cstddef>
#include "frame.h"

namespace network_counter {

const size_t BUFFER_SIZE = 65536;

class SnifferManager
{
public:
    SnifferManager();

    void Process();

private:
    int socket;
    std::array<std::byte, BUFFER_SIZE> rawBuffer;

};

void info_printer(const EthernetFrame& frame, std::ostream& out)
{
    out << "Ethernet frame:\n";
    out << "Destination: " << frame.GetDstMacStr() << "; ";
    out << "Source: " << frame.GetDstMacStr() << "; ";
    out << "Protocol: " << frame.GetProtocol() << "; ";
    out << std::endl;
}

}
