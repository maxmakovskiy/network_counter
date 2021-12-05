#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include <ctime>
#include <vector>

namespace network_counter {

namespace fs = std::filesystem;

class File
{
public:
    File(const std::string& fileName);
    void WriteString(const std::string& str);

private:
    fs::path fileName;
    std::ofstream fileStream;
    
    void AddTimestamp();
};

}

