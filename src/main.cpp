#include <iostream>
#include <string>
#include "manager.h"
//#include <sqlite3.h>

/*
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
*/

int main()
{

    network_counter::SnifferManager manager;
//    manager.AssignFile("log1.txt");
    manager.Process();

/*
    sqlite3 *db;
    char *zErrMsg = 0;
    int error_code;
    std::string query = "CREATE TABLE COMPANY("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "NAME           TEXT    NOT NULL," \
      "AGE            INT     NOT NULL," \
      "ADDRESS        CHAR(50)," \
      "SALARY         REAL );";

    error_code = sqlite3_open("test.db", &db);
    if(error_code) // SQLITE_OK == 0
    {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return(1);
    }

    error_code = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
    if(error_code != SQLITE_OK)
    {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
*/


    return 0;
}


