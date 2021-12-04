#include "ntfile.h"

namespace network_counter {

NTFile::NTFile(const std::string& fileName)
    : fileName(fileName)
    , fileStream(fileName)
{
    if (!fileStream.is_open())
    {
        std::cerr << "Could not open the file: " << fileName << std::endl;
        exit(1);
    }
}

void NTFile::WriteString(const std::string& str)
{
    fileStream << str << '\n';
}

}


