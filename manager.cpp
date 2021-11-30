#include "manager.h"

namespace network_counter {

SnifferManager::SnifferManager()
{
    socketDescr = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (socketDescr == -1) 
    {
        std::cerr << "socket was not created\n";
        std::cerr << std::strerror(errno) << std::endl;

        exit(1);
    }
    
    rawBuffer = (unsigned char*)malloc(BUFFER_SIZE);

    std::fill(rawBuffer, rawBuffer+BUFFER_SIZE, 0);
}

SnifferManager::~SnifferManager()
{
    close(socketDescr);
    free(rawBuffer);
}

void
SnifferManager::Process()
{
    struct sockaddr addrFrom;
    int addrFromLen = sizeof(addrFrom);
    int currentBuffLen = -1;

    while(true)
    {
        currentBuffLen = recvfrom(socketDescr, rawBuffer,
                BUFFER_SIZE, 0, &addrFrom, (socklen_t*)&addrFromLen);
        if (currentBuffLen == -1)
        {
            std::cerr << "error by using recvfrom" << std::endl;
            exit(1);
        }

        RawFrame frame(rawBuffer, currentBuffLen);
    
        std::cout << frame;
    }

}

}

