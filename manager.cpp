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

    memset(rawBuffer, 0, BUFFER_SIZE);
}

SnifferManager::~SnifferManager()
{
    close(socketDescr);
    free(rawBuffer);
}


void
SnifferManager::AssignFile
(const std::string& filename)
{
    //file = std::make_optional<File>(filename);
    std::cout << filename;
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
#ifdef VISUAL_DEBUG        
        std::cout << std::strerror(errno) << std::endl;
        std::cout << "RAW BUFFER LENGTH: " << currentBuffLen << '\n';
#endif
        if (currentBuffLen == -1)
        {
            std::cerr << "error by using recvfrom" << std::endl;
            exit(1);
        }

        RawFrame frame(rawBuffer, currentBuffLen);
/*   
        if(file.has_value()) 
        {
            std::stringstream ss;
            ss << frame;
            file.WriteStrings(ss.str());
        }
        else 
        {*/
            std::cout << frame;
//        }

    }

}

}

