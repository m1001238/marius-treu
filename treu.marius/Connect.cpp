#include "Connect.h"

namespace programm {
    Connect::Connect(char *HOST,int PORT,char *CHANNEL) {
        printf("%s->%i->%s\n",string(HOST).c_str(),PORT,string(CHANNEL).c_str());

        this->HOST = HOST;
        this->PORT = PORT;
        this->BUF_SIZE = 1024;

        this->BOTNAME = "NekkidBot";
        this->PASSWORD = "123hallo";
        this->CHANNEL = CHANNEL;

        this->irc_connect();
        this->irc_identify();
    }
    Connect::~Connect() {
        this->irc_disconnect();
    }

    string Connect::run() {
        char *buffer = new char[this->BUF_SIZE+1];
        memset(buffer, 0, BUF_SIZE+1);
        if (recv(sockfd, buffer, this->BUF_SIZE*sizeof(char), 0) < 0) { perror("recv()"); this->irc_disconnect(); exit(1); }
        cout << buffer;
        string bufStr(buffer);
        delete buffer;
        return bufStr;
    }

    void Connect::irc_connect() {
        #ifdef WIN32
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2,0), &wsa) != 0) exit(1);
        #endif
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) { perror("socket"); this->irc_disconnect(); exit(1); }

        hostent *hp = gethostbyname(HOST);
        if(!hp) { cerr << "gethostbyname()" << endl; this->irc_disconnect(); exit(1);}

        sockaddr_in sin;
        memset((char*)&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        memcpy((char*)&sin.sin_addr, hp->h_addr, hp->h_length);
        sin.sin_port = htons(PORT);
        memset(&(sin.sin_zero), 0, 8*sizeof(char));

        if (connect(sockfd, (sockaddr*) &sin, sizeof(sin)) == -1) { perror("connect"); this->irc_disconnect(); exit(1); }
    }

    void Connect::irc_identify() {
        s2u(("NICK " + this->BOTNAME + "\r\n").c_str());
        s2u(("USER " + this->BOTNAME + " 0 0  :" + this->BOTNAME + "\r\n").c_str());
        s2u(("PRIVMSG NickServ IDENTIFY " + this->BOTNAME + " " + this->PASSWORD + "\r\n").c_str());
        s2u(("JOIN #" + this->CHANNEL + "\r\n").c_str());
        s2u(("PRIVMSG #" + this->CHANNEL + ":Hallo!\r\n").c_str());
    }

    void Connect::irc_disconnect() {
        #ifdef WIN32
        closesocket(sockfd);
        WSACleanup();
        #else
        close(sockfd);
        #endif
    }

    void Connect::s2u(const char *msg) {
        send(sockfd, msg, strlen(msg), 0);
    }

    void Connect::ping_parse(const string &buffer) {
        size_t pingPos = buffer.find("PING");
        if (pingPos != string::npos) {
            string pong("PONG"+buffer.substr(pingPos+4)+"\r\n");
            cout << pong;
            s2u(pong.c_str());
        }
    }

    string Connect::getBotname() {
        return this->BOTNAME;
    }

    string Connect::getChannel() {
        return this->CHANNEL;
    }

    void Connect::setBotname(string name) {
        this->BOTNAME = name;
    }
}
