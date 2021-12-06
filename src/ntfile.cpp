#include "ntfile.h"

namespace network_counter {

File::File(const std::string& fileName)
    : fileName(fileName)
    , fileStream(fileName)
{
    fileStream << "Report was generated: ";
    AddTimestamp();
}

void File::WriteString(const std::string& str)
{
    fileStream << str << '\n';
}

void File::AddTimestamp()
{
    std::time_t currentTime = std::time(nullptr);
    fileStream << std::asctime(std::localtime(&currentTime)) << '\n';
}

}


