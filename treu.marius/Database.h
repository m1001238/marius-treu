#include <stdio.h>
#include <string>
#include <sqlite3.h>
#include <ctime>
#include <sstream>
#include "Utilities.h"

using namespace std;
using namespace utilities;

namespace programm {
    class Database {
        private:
            sqlite3 *conn;
            int error;
        public:
            Database();
            ~Database();
            void insertIntoDB(string username,string msg);
            string showAllLoggin();
            string selectLastSeen(string name);
    };
}
