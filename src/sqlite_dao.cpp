#include "sqlite_dao.h"


namespace network_counter {
SQLite::SQLite()
{
    errorCode = sqlite3_open(":memory:", &dbObj);
    if (errorCode)
    {
        std::cerr << "Can't open DB in memory " << sqlite3_errmsg(dbObj) << std::endl;
        sqlite3_close(dbObj);

#ifdef SQL_DEBUG_EXCEPTION
        throw std::invalid_argument("can't create DB in memory");
#endif
        exit(1);
    }
    
    CreateEmptyTable();
}

SQLite::~SQLite()
{
    sqlite3_close(dbObj);
}

bool SQLite::AddRow(const UnpackedFrame& frame)
{
    std::string query = "INSERT INTO PACKETS (DstMAC,SrcMAC,EtherType,DstIP,SrcIP,ProtocolIP) VALUES ('";
    query.append(frame.ethHdr.dstMac + "', '");
    query.append(frame.ethHdr.srcMac + "', ");
    query.append(std::to_string(frame.ethHdr.type) + ", '");
    query.append(frame.ipHdr.dstIP + "', '");
    query.append(frame.ipHdr.srcIP + "', ");
    query.append(std::to_string(frame.ipHdr.protocol) + ");");
    
    errorCode = sqlite3_exec(dbObj, query.c_str(), NULL,
            0, &errorMessage);
    if (errorCode != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);

#ifdef SQL_DEBUG_EXCEPTION
        throw std::invalid_argument("add_row");
#endif

        return false;
    }
    
    return true;
}

void SQLite::CreateEmptyTable()
{
    std::string createTableQuery = "CREATE TABLE PACKETS("  \
      "ID INTEGER PRIMARY KEY    AUTOINCREMENT," \
      "DstMAC   TEXT    NOT NULL," \
      "SrcMAC   TEXT    NOT NULL," \
      "EtherType   INT    NOT NULL," \
      "DstIP    TEXT    NOT NULL," \
      "SrcIP   TEXT    NOT NULL," \
      "ProtocolIP   INT    NOT NULL);";

    errorCode = sqlite3_exec(dbObj, createTableQuery.c_str(), NULL,
            0, &errorMessage);
    if(errorCode)
    {
        std::cerr << "SQL error: " << errorMessage << std::endl; 
        sqlite3_free(errorMessage);

#ifdef SQL_DEBUG_EXCEPTION
        throw std::invalid_argument("create_empty_table");
#endif
    }
}

//std::vector<UnpackedFrame>
void
SQLite::LoadFramesByIPproto
(uint32_t protocolNumber)
{
    std::string selectQuery = "SELECT * FROM PACKETS WHERE ProtocolIP = "
        + std::to_string(protocolNumber);

    errorCode = sqlite3_prepare_v2(dbObj, selectQuery.c_str(),
                -1, &selectByIPprotoStmt, NULL);
    if(errorCode != SQLITE_OK) 
    {
        std::cerr << "SQL error: " << sqlite3_errmsg(dbObj) << std::endl;
        sqlite3_close(dbObj);
        sqlite3_finalize(selectByIPprotoStmt);

#ifdef SQL_DEBUG_EXCEPTION
        throw std::invalid_argument("get_frame_by_IP_proto");
#endif

        return;
    }

    // execute sql statement, and while there are rows returned, print ID
    int ret_code = 0;
    while((ret_code = sqlite3_step(selectByIPprotoStmt)) == SQLITE_ROW) 
    {
        UnpackedFrame frame;

        auto tmpDstMac = 
            reinterpret_cast<const char*>(sqlite3_column_text(
                        selectByIPprotoStmt, 1));
        frame.ethHdr.dstMac = std::string(tmpDstMac); // string

        auto tmpSrcMac = 
            reinterpret_cast<const char*>(sqlite3_column_text(
                        selectByIPprotoStmt, 2));
        frame.ethHdr.srcMac = std::string(tmpSrcMac); // string

        auto ethType = sqlite3_column_int(selectByIPprotoStmt, 3);
        frame.ethHdr.type = static_cast<uint16_t>(ethType); // uint16_t

        auto tmpDstIP = 
            reinterpret_cast<const char*>(sqlite3_column_text(
                        selectByIPprotoStmt, 4));
        frame.ipHdr.dstIP = std::string(tmpDstIP); // string

        auto tmpSrcIP =
            reinterpret_cast<const char*>(sqlite3_column_text(
                        selectByIPprotoStmt, 5));
        frame.ipHdr.srcIP = std::string(tmpSrcIP); // string

        auto ipProto = sqlite3_column_int(selectByIPprotoStmt, 6);
        frame.ipHdr.protocol = static_cast<uint32_t>(ipProto); // uint32_t
        
        buffer.push_back(frame);
    }

}

std::vector<UnpackedFrame> 
SQLite::GetSelectedPackets()
{
    return buffer;
}

}



