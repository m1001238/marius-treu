#include "Database.h"

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

    //this->error = sqlite3_exec(this->conn,query.str().c_str(),0,0,0);
    this->error = sqlite3_exec(this->conn,query.str().c_str(),0,0,0);

    if(this->error) {
        puts("SQL insert hat nicht geklappt");
    }
    else {
        puts("Logge mit!");
    }
}

void Database::showAllLoggin() {
    int rec_count = 0;
    const char* errMSG;
    const char* tail;
    sqlite3_stmt *res;

    this->error = sqlite3_prepare_v2(this->conn,"select name,date,msg from logging",1000, &res, &tail);
    while (sqlite3_step(res) == SQLITE_ROW) {
        printf("%s|", sqlite3_column_text(res, 0));
        printf("%s|", sqlite3_column_text(res, 1));
        printf("%s|", sqlite3_column_text(res, 2));
        printf("%u\n", sqlite3_column_int(res, 3));

        rec_count++;
    }
}
