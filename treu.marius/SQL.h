#include <stdio.h>
#include <string>
#include <sqlite3.h>
#include <ctime>
#include <sstream>


class SQL {
    private:
        sqlite3 *conn;
        sqlite3_stmt *res;
        int error;
        char* errMSG;
        char* tail;
    public:
        SQL(string filename);
        ~SQL();
        void insertIntoDB(string username,string msg);
        void selectOutOfDB(string username,int timestamp,string msg);
};
