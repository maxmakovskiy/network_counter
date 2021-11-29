#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <array>
#include <cstddef>
#include <cstring>
#include "frame.h"

namespace network_counter {

const size_t BUFFER_SIZE = 65536;

class SnifferManager
{
public:
    SnifferManager();

    void Process();

private:
    int socketDescr;
    std::array<std::byte, BUFFER_SIZE> rawBuffer;

};

void info_printer(const EthernetFrame& frame, std::ostream& out);

}
