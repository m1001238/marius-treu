#include "Database.h"

namespace programm {
    Database::Database() {
        this->error = sqlite3_open(("datenbank"), &this->conn);
        if (this->error) {
            puts("SQL kann nicht aufgerufen werden");
        }
        this->error = sqlite3_exec(this->conn,"create table loggin (username varchar(50),date integer,msg varchar(200))",0,0,0);
        if (this->error) {
            puts("Datenbank konnte nicht erstellt werden");
        }
    }
    Database::~Database() {
    }
    void Database::insertIntoDB(string username,string msg) {
        int timestamp = time(0);
        stringstream query;
        query << "insert into loggin (username,date,msg) values ('" << username << "', " << timestamp << ", '" << msg << "')";

        this->error = sqlite3_exec(this->conn,query.str().c_str(),0,0,0);
        if(this->error) {
            puts("SQL insert hat nicht geklappt");
        }
        else {
            puts("Logge mit!");
        }
    }

    string Database::showAllLoggin() {
        int rec_count = 0;
        const char* errMSG;
        const char* tail;
        sqlite3_stmt *res;

        this->error = sqlite3_prepare_v2(this->conn,"SELECT username,date,msg FROM loggin ORDER BY date DESC LIMIT 10",1000, &res, &tail);
        if(this->error) {
            puts("hat wohl net geplappt mit dem auslesen...");
        }
        stringstream output;
        while (sqlite3_step(res) == SQLITE_ROW) {
            output << "Username: " << sqlite3_column_text(res, 0) << " Date: " << utilities::TimeTools::getTimeStr( sqlite3_column_int(res, 1) ) << " MSG/Action: " << sqlite3_column_text(res, 2) << "\n";
            rec_count++;
        }
        return output.str();
    }
    string Database::selectLastSeen(string name) {
        int rec_count = 0;
        const char* errMSG;
        const char* tail;
        sqlite3_stmt *res;
        stringstream query;

        query << "SELECT username,date,msg FROM loggin WHERE username='"<<name<<"' ORDER BY date DESC LIMIT 1";

        this->error = sqlite3_prepare_v2(this->conn,query.str().c_str(),1000, &res, &tail);
        if(this->error) {
            puts("hat wohl net geplappt mit dem auslesen...");
        }
        stringstream output;
        while (sqlite3_step(res) == SQLITE_ROW) {
            output << "Username: " << sqlite3_column_text(res, 0) << " last seen on: " << utilities::TimeTools::getTimeStr( sqlite3_column_int(res, 1) );
            return output.str();

            rec_count++;
        }
        return string("Den user gibt es hier drinnen net :(");
    }
}
