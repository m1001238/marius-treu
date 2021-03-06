#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>

using namespace std;

int main() {
    int sock;
    struct sockaddr_in server;
    char message[1024], response[1024];

    /* Socket erzeugen */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket konnte nicht erzeugt werden!");
        return 1;
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(1234);

    /* Verbindung aufbauen */
    if (connect(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) < 0) {
        perror("Verbindung fehlgeschlagen!");
        return 1;
    } printf("Verbindung hergestellt!\n");

    char username[255];
    printf("Username: "); cin.getline(username, 255);

    /* Sende Username */
    if (write(sock, username, strlen(username)) < 0) {
        perror("Senden fehlgeschlagen!");
        return 1;
    }

    int child_pid = fork();

    while(1) {
        if (child_pid == 0) {
            //sende Nachticht an server
            cout << username << "> ";
            cin.getline(message, 1024);
            /* Sende Daten */
            if (write(sock, message, strlen(message)+1) < 0) {
                perror("Senden fehlgeschlagen!");
                return 1;
            }
        } else if (child_pid > 0) {
            /* Daten empfangen
            if (read(sock, response, 1024) < 0) {
                perror("Lesen fehlgeschlagen!");
                return 1;
            }
            printf("Reply: %s\n", response);*/
        } else {
            perror("forking failed...");
            break;
        }
    }
    close(sock);
    return 0;
}
