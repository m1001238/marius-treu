#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <fstream>

#include "Connect.h"
#include "Database.h"

using namespace std;

namespace programm {
    class ConfigData {
        public:
            char HOST[100];
            int PORT;
            char CHANNEL[100];
            ConfigData() {}
            ConfigData(char* HOST,int PORT,char* CHANNEL) {
                strcpy(this->HOST, HOST);
                this->PORT = PORT;
                strcpy(this->CHANNEL, CHANNEL);
            }
    };

    class Config {
        public:
            ConfigData* configData;
            Config();
            Config(char* HOST,int PORT,char* CHANNEL);
            ~Config();
            void setConfigFile(char* HOST,int PORT,char* CHANNEL);
    };

    class Bot {
        private:
            bool enableLogging;
            Config* config;
            Connect* connect;
            Database* db;
        public:
            Bot();
            Bot(char *HOST,int PORT,char *CHANNEL);
            ~Bot();
            void run();
            int irc_parse(string buffer);
            int interpret_parse(string buffer);
            int bot_functions(string sender, string message);

            void options();
            void loggin(string sender,string message);
            void showlog();
            void lastSeen(string name);
            void changeName(string name);
            void joinChannel(string chan);
            void leaveChannel(string chan);
            void changeTopic(string chan, string topic);
            void getCurTime();
            void love(string du,string partner);
    };
}
