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
    
    rawBuffer = (unsigned char*)malloc(sizeof(unsigned char) * BUFFER_SIZE);

    std::fill(rawBuffer, rawBuffer+BUFFER_SIZE, 0);
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

        EthernetFrame frame(rawBuffer);

        info_printer(frame, std::cout);
    }

}

void info_printer(const EthernetFrame& frame, std::ostream& out)
{
    out << "Ethernet frame:\n";
    out << "Destination: " << frame.GetDstMacStr() << "; ";
    out << "Source: " << frame.GetDstMacStr() << "; ";
    out << "Protocol: " << frame.GetProtocol() << "; ";
    out << std::endl;
}



}

