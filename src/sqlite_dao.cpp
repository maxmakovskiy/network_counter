#include "sqlite_dao.h"


namespace network_counter {

SQLite::SQLite()
{
    errorCode = sqlite3_open(":memory:", &dbObj);
    if (errorCode)
    {
        std::cerr << "Can't open DB in memory " << sqlite3_errmsg(dbObj) << std::endl;
        sqlite3_close(dbObj);
        exit(1);
    }
    
    CreateEmtyTable();
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

    std::string insertQuery = "INSERT INTO COMPANY (ID,SessNum,Timestamp,DstMAC,SrcMAC,EtherType,DstIP,SrcIP,TotalLength) "
        + "VALUES (" + id + ", " + sessNum + ", "
        + timestamp + ", '" + frame.ethHdr.dstMac + "', "
        + "'" + frame.ethHdr.srcMac + "', "
        + frame.ethHdr.type + ", "
        + "'" + frame.ipHdr.dstIP + "', "
        + "'" + frame.ipHdr.srcIP + "', "
        + frame.ipHdr.protocol
        + ");";
    
    errorCode = sqlite3_exec(dbObj, insertQuery.c_str(), StatsCallback,
            0, errorMessage);
    if (errorCode != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

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
      "TotalLength   INT    NOT NULL,";

    errorCode = sqlite3_exec(dbObj, createTableQuery.c_str(), StatsCallback
            0, &errorMessage);
    if(errorCode)
    {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}

int
SQLite::CreateTableCallback
(void *NotUsed, int argc, char **argv, char **azColName) 
{
   int i;
   for(i = 0; i<argc; i++) {
       std::cout << azColName[i] << " = "
           << (argv[i] ? argv[i] : "NULL") << '\n';
   }
   return 0;
}

}



