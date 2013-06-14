#include "SQL.h"

SQL::SQL(string filename) {
    /*this->error = sqlite3_open(filename.c_str(), &this->conn);
    if (this->error) {
        puts("SQL kann nicht aufgerufen werden");
        exit(0);
    }
    this->error = sqlite3_exec(this->conn,"create table if not exists loggin (username varchar(50),msg_date date,msg varchar(1024))",0,0,0);
    if (this->error) {
        puts("Datenbank konnte nicht erstellt werden");
        exit(0);
    }*/
}
SQL::~SQL() {
}

void SQL::insertIntoDB(string username,string msg) {
    /*int timestamp = time(0);
    printf("%i",timestamp);
    stringstream query;
    query << "insert into logging (username,date,msg) values ('" << username << "', FROM_UNIXTIME(" << timestamp << "), '" << msg << "')";

    printf("%s",query.str().c_str());
    sql.error = sqlite3_exec(sql.conn,query.str().c_str(),0,0,0);
    if(sql.error) {
        puts("SQL insert hat nicht geklappt");
        exit(0);
    }
    else {
        puts("klappt wirklich Oo");
        printf("%s",query.str().c_str());
    }*/
}
void SQL::selectOutOfDB(string username,int timestamp,string msg) {
    /*int rec_count = 0;
    sql_error = sqlite3_prepare_v2(conn,"select name,date,msg from logging",1000, &res, &tail);
    while (sqlite3_step(res) == SQLITE_ROW) {
        printf("%s|", sqlite3_column_text(res, 0));
        printf("%s|", sqlite3_column_text(res, 1));
        printf("%s|", sqlite3_column_text(res, 2));
        printf("%u\n", sqlite3_column_int(res, 3));

        rec_count++;
    }*/
}
