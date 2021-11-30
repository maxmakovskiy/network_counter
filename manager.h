#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <malloc.h>
#include <cstring>
#include <unistd.h>
#include "frame.h"

namespace network_counter {

const size_t BUFFER_SIZE = 65536;

class SnifferManager
{
public:
    SnifferManager();
    ~SnifferManager();

    void Process();

private:
    int socketDescr;
    unsigned char* rawBuffer;

};

}
