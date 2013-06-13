#include <stdio.h>
#include <string>
#include "Connect.h"
#include "Database.h"

class Bot {
    private:
        bool enableLogging;
        Connect *connect;
        Database *db;
    public:
        Bot(char *HOST,int PORT,char *CHANNEL);
        ~Bot();
        int irc_parse(string buffer);
        int interpret_parse(string buffer);
        int bot_functions(string sender, string message);

        int options();
        int changeName(string name);
        void loggin(string sender,string message);
        void showlog();
};
