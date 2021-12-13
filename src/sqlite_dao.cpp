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
    std::string query = "INSERT INTO PACKETS (DstMAC,SrcMAC,EtherType,DstIP,SrcIP,ProtocolIP, IPpayload) VALUES ('";
    query.append(frame.ethHdr.dstMac + "', '");
    query.append(frame.ethHdr.srcMac + "', ");
    query.append(std::to_string(frame.ethHdr.type) + ", '");
    query.append(frame.ipHdr.dstIP + "', '");
    query.append(frame.ipHdr.srcIP + "', ");
    query.append(std::to_string(frame.ipHdr.protocol) + ", ?);");
   
    errorCode = sqlite3_prepare_v2(dbObj, query.c_str(),
                -1, &insertRowStmt, NULL);
    if(errorCode != SQLITE_OK) 
    {
        std::cerr << "SQL error: " << sqlite3_errmsg(dbObj) << std::endl;
        sqlite3_close(dbObj);
        sqlite3_finalize(insertRowStmt);
#ifdef SQL_DEBUG_EXCEPTION
        throw std::invalid_argument("add_row");
#endif
        return false;
    }

    unsigned char* data;
    // Boiler plate code
    // Needs for determine size of buffer
    // which is using as temporary storage
    // frame -> serialize -> data -> save data as blob
    size_t counter = 0;
    if (std::holds_alternative<ICMP>(frame.ipPayload)) { // ICMP
        data = (unsigned char*)malloc(1 + 4 + 4);
        counter = 9;
    } else if (std::holds_alternative<IGMP>(frame.ipPayload)) { // IGMP
        auto igmpRef = std::get<IGMP>(frame.ipPayload);
        data = (unsigned char*)malloc(1 + 4 + 4 + 1 + igmpRef.groupAddr.size());
        counter = 1 + 4 + 4 + 1 + igmpRef.groupAddr.size();
    } else { // TCP or UDP
        data = (unsigned char*)malloc(1 + 4 + 4 + 4);
        counter = 1 + 4 + 4 + 4;
    }
    serializeIPpayload(&frame.ipPayload, data);// your data
    sqlite3_bind_blob(insertRowStmt, 1, data, counter, SQLITE_STATIC);

    if (sqlite3_step(insertRowStmt) != SQLITE_DONE)
    {
        std::cerr << "SQL error: " << sqlite3_errmsg(dbObj) << std::endl;
        sqlite3_close(dbObj);
        sqlite3_finalize(insertRowStmt);
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
      "ProtocolIP   INT    NOT NULL," \
      "IPpayload   BLOB    NOT NULL);";

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

    // execute sql statement, and while there are rows returned
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
       
        const unsigned char* blobData =
            reinterpret_cast<const unsigned char*>(
                    sqlite3_column_blob(selectByIPprotoStmt, 7));
        frame.ipPayload = deserializeIPpayload(blobData); 

        buffer.push_back(frame);
    }

    sqlite3_finalize(selectByIPprotoStmt);
}

std::vector<UnpackedFrame> 
SQLite::GetSelectedPackets()
{
    return buffer;
}

}



