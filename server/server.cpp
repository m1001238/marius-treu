#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <vector>

#include <sqlite3.h>
#include <ctime>

#include <sstream>

using namespace std;

#define PORT 1234
#define BUF_SIZE 1024

struct SQL {
    sqlite3 *conn;
    sqlite3_stmt *res;
    int error;
    const char *errMSG;
    const char *tail;

    SQL(string filename) {
        error = 0;

        this->error = sqlite3_open(filename.c_str(), &this->conn);
        if (this->error) {
            puts("SQL kann nicht aufgerufen werden");
            exit(0);
        }
    }
};

void receiveClientMessage(SQL sql,int sock2,char buf[]);
void insertIntoDB(SQL sql,string username,string msg);

int main() {
    int sock1, sock2;
    struct sockaddr_in server;
    char buf[BUF_SIZE];

    SQL sql("logging.sl3");
    //tabelle erstellen
    sql.error = sqlite3_exec(sql.conn,"create table if not exists logging (username varchar(50),msg_date date,msg varchar(1024))",0,0,0);
    if (sql.error) {
        puts("Datenbank konnte nicht erstellt werden");
        exit(0);
    }

    server.sin_family       = AF_INET;      // Protokollfamilie
    server.sin_addr.s_addr  = htonl(INADDR_ANY);   // Interface
    server.sin_port         = htons(PORT);  // Port

    /* Stream Socket AF_INET erzeugen */
    sock1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sock1 < 0) {
        perror("Socket wurde nicht erzeugt!");
        exit(1);
    }
    /* Socket konfigurieren */
    if (bind(sock1, (struct sockaddr*) &server, sizeof(struct sockaddr_in))) {
        perror("Socket konnte nicht konfiguriert werden!");
        exit(1);
    }
    /* Socket in den listen-Zustand versetzen */
    listen(sock1, 5);

    printf("Socket-Server bereit\n");

    while(1) {
        printf("Erwarte eingehende Verbindung\n");
        /* Verbindung akzeptieren */
        sock2 = accept(sock1, 0, 0);    // BLOCKIERT!
        cout << "sock: " << sock2 << endl;

        int child_pid;
        if((child_pid = fork()) == 0)
            receiveClientMessage(sql,sock2,buf);
        else if(child_pid > 0) {
            // Elternprozess
            cout << "Hi, ich bin der Vater\n";
        } else {
            perror("Fork fehlgeschlagen!");
            close(sock2);
        }
    }
    close(sock1);
    return 0;
}




void receiveClientMessage(SQL sql,int sock2,char buf[]) {
    char username[255];
    int username_length;

    if (sock2 < 0) {
        perror("Fehler bei accept()");
        return;
    }
    else {
        if (username_length = read(sock2, username, 255)) {
            if (username_length < 0)
                perror("Fehler beim Lesen");
            else {
                username[username_length] = '\0';
                write(1, username, username_length);
                write(1, " hat sich angemeldet.\n", 32);
            }
        }
    }
    int received;
    while (received = read(sock2, buf, BUF_SIZE)) {
        //username + message ausgabe
        buf[received] = '\0';
        write(1, username, username_length);
        write(1, " sagt: ", 7);
        write(1, buf, received+1);
        write(1, "\n", 1);

        char *str = new char[received];
        snprintf(str, received, "%s", buf);
        insertIntoDB(sql,string(username),string(str));
    }
    printf("Ende der Kommunikation\n");
    close(sock2);
}
void insertIntoDB(SQL sql,string username,string msg) {
    int timestamp = time(0);
    printf("%i",timestamp);
    stringstream query;
    query << "insert into logging (username,date,msg) values ('" << username << "', FROM_UNIXTIME(" << timestamp << "), '" << msg << "')";

    printf("%s",query.str().c_str());
    /*sql.error = sqlite3_exec(sql.conn,query.str().c_str(),0,0,0);
    if(sql.error) {
        puts("SQL insert hat nicht geklappt");
        exit(0);
    }
    else {
        puts("klappt wirklich Oo");
        printf("%s",query.str().c_str());
    }*/
}
/*void selectOutOfDB(string username,int timestamp,string msg) {
    int rec_count = 0;
    sql_error = sqlite3_prepare_v2(conn,"select name,date,msg from logging",1000, &res, &tail);
    while (sqlite3_step(res) == SQLITE_ROW) {
        printf("%s|", sqlite3_column_text(res, 0));
        printf("%s|", sqlite3_column_text(res, 1));
        printf("%s|", sqlite3_column_text(res, 2));
        printf("%u\n", sqlite3_column_int(res, 3));

        rec_count++;
    }
}*/
