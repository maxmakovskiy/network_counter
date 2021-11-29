#include "manager.h"

namespace network_counter {

SnifferManager::SnifferManager()
{
    socket = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
    if (socket == -1) 
    {
        std::cerr << "socket was not created" << std::endl;
        exit(1);
    }

    std::fill(rawBuffer.begin(), rawBuffer.end(), std::byte{0});
}

SnifferManager::Process()
{
    struct sockaddr addrFrom;
    int addrFromLen = sizeof(addrFrom);
    int currentBuffLen = -1;

    while(true)
    {
        currentBuffLen = recvfrom(socket, rawBuffer.data(),
                BUFFER_SIZE, 0, &addrFrom, (socklen_t*)&addrFromLen);
        if (currentBuffLen == -1)
        {
            std::cerr << "error by using recvfrom" << std::endl;
            exit(1);
        }

        EthernetFrame frame(std::vector<std::byte>(rawBuffer.begin(),
                    rawBuffer.end());

        info_printer(frame, std::cout);
    }

}





}

