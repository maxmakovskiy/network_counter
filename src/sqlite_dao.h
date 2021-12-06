#pragma once

#include <sqlite3.h>
//#include <ctime>
#include <vector>
#include <cstdint>
#include "unpacked_frame.h"

#ifdef SQL_DEBUG_EXCEPTION
#include <exception>
#endif

namespace network_counter {

/**
 * Make DB in memory for save info about current program usage session.
 *
 * Design database scheme
 1. 
| Primary key | Session number | Timestamp |
 2.
| Primary |        Ethernet header        |            IPv4 header           |
| key     | Dst MAC | Src MAC | EtherType | Dst IP  | Src IP  | Total Length |
 *
 * **/

class SQLite
{
public:
    SQLite();
    ~SQLite();
    
    bool AddRow(const UnpackedFrame& frame);
    void GetFramesByIPproto(uint32_t protocolNumber);
//    std::vector<UnpackedFrame> GetFramesByIPproto(uint32_t protocolNumber);

/*
    std::vector<UnpackedFrame> GetFramesByEtherType(size_t etherType);
    std::vector<UnpackedFrame> GetFramesBySessionNumber(size_t sessionNumber);
    std::vector<UnpackedFrame> GetFramesByMac(size_t mac);
    std::vector<UnpackedFrame> GetFramesByIP(size_t ip);
    // more compex condition: low =< rawLength =< high
    std::vector<UnpackedFrame> GetFramesByRawLengthRange(size_t sessionNumber);
    // more compex condition: from =< timestamp =< to 
    std::vector<UnpackedFrame> GetFramesByTimestamp(time_t from, time_t to);
*/

private:
    sqlite3* dbObj;
    int errorCode;
    char* errorMessage; 
    size_t id = 1;

    void CreateEmptyTable();

    static int StatsCallback(void* NotUsed, int argc, char **argv, char **azColName);
    static int GetFramesByFilter(void* data, int argc, char **argv, char **columns);

};

}

