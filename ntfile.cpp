#include "ntfile.h"

namespace network_counter {

File::File(const std::string& fileName)
    : fileName(fileName)
    , fileStream(fileName)
{
}

void File::WriteString(const std::string& str)
{
    fileStream << str << '\n';
}

}


