#include <stdio.h>
#include <string>
#include <sqlite3.h>
#include <ctime>
#include <sstream>

using namespace std;

class Database {
    private:
        sqlite3 *conn;
        int error;
    public:
        Database();
        ~Database();
        void insertIntoDB(string username,string msg);
        void showAllLoggin();
};
