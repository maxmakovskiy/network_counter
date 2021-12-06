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
    // MOCK!!!!!!!!!!!!!!!
    const size_t sessNum = 1;
    const size_t timestamp = 10000;

    std::string insertQuery = "INSERT INTO PACKETS (ID,SessNum,Timestamp,DstMAC,SrcMAC,EtherType,DstIP,SrcIP,ProtocolIP) VALUES (" + std::to_string(id) + ", " + std::to_string(sessNum) + ", " + std::to_string(timestamp) + ", '" + frame.ethHdr.dstMac + "', " + "'" + frame.ethHdr.srcMac + "', " + std::to_string(frame.ethHdr.type) + ", " + "'" + frame.ipHdr.dstIP + "', " + "'" + frame.ipHdr.srcIP + "', " + std::to_string(frame.ipHdr.protocol) + ");";
    
    errorCode = sqlite3_exec(dbObj, insertQuery.c_str(), NULL/*StatsCallback*/,
            0, &errorMessage);
    if (errorCode != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);

#ifdef SQL_DEBUG_EXCEPTION
        throw std::invalid_argument("add_row");
#endif

        //return false;
    }
    
    ++id;
    return true;
}

void SQLite::CreateEmptyTable()
{
    std::string createTableQuery = "CREATE TABLE PACKETS("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "SessNum   INT    NOT NULL," \
      "Timestamp   INT    NOT NULL," \
      "DstMAC   TEXT    NOT NULL," \
      "SrcMAC   TEXT    NOT NULL," \
      "EtherType   INT    NOT NULL," \
      "DstIP    TEXT    NOT NULL," \
      "SrcIP   TEXT    NOT NULL," \
      "ProtocolIP   INT    NOT NULL);";

    errorCode = sqlite3_exec(dbObj, createTableQuery.c_str(), NULL/*StatsCallback*/, 0, &errorMessage);
    if(errorCode)
    {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);

#ifdef SQL_DEBUG_EXCEPTION
        throw std::invalid_argument("create_empty_table");
#endif
    }
}

int
SQLite::StatsCallback
(void *NotUsed, int argc, char **argv, char **azColName) 
{
   int i;
   for(i = 0; i<argc; i++) {
       std::cout << azColName[i] << " = "
           << (argv[i] ? argv[i] : "NULL") << '\n';
   }
   return 0;
}

//std::vector<UnpackedFrame>
void
SQLite::GetFramesByIPproto
(uint32_t protocolNumber)
{
//    char* data = "Callback function called";
 
    std::string selectQuery = "SELECT * FROM PACKETS WHERE ProtocolIP = "
        + std::to_string(protocolNumber);

    errorCode = sqlite3_exec(dbObj, selectQuery.c_str(), GetFramesByFilter,
        NULL/*data*/, &errorMessage);

    if(errorCode != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);

#ifdef SQL_DEBUG_EXCEPTION
        throw std::invalid_argument("get_frame_by_IP_proto");
#endif
    }
}


int
SQLite::GetFramesByFilter
(void* data, int argc, char** argv, char **columns)
{
   int i;
   //std::cerr << std::string(reinterpret_cast<char*>(data)) << '\n';
   
   for(i = 0; i<argc; i++) {
        const char* argvi = argv[i] ? argv[i] : "NULL";
        std::cout << std::string(columns[i]) << " = "
            << std::string(argvi) << std::endl;
   }

   return 0;
}

}



