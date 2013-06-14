#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <stdio.h>

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

namespace programm {
    class Connect {
        private:
            //variables
             #ifdef WIN32
            SOCKET          sockfd;
            #else
            int             sockfd;
            #endif

            unsigned int    BUF_SIZE;
            int             PORT;
            char*           HOST;

            string          BOTNAME;
            string          PASSWORD;
            string          CHANNEL;
            vector<string>  bot_witze;

            //methods
            void            irc_connect();
            void            irc_disconnect();
            void            irc_identify();
        public:
            //variables
            //methods
            Connect(char *HOST,int PORT,char *CHANNEL);
            ~Connect();
            string          run();
            void            ping_parse(const string &buffer);
            void            s2u(const char *msg);
            string          getBotname();
            string          getChannel();
            void            setBotname(string name);
    };
}
