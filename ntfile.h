#pragma once

#include <fstream>
#include <string>
#include <iostream>

namespace network_counter {

class NTFile
{
public:
    NTFile(const std::string& fileName);

    void WriteString(const std::string& str);

private:
    std::string fileName;
    std::ofstream fileStream;
};

}

