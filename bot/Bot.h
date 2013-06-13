#include <stdio.h>
#include <string>
#include "Connect.h"
#include "Database.h"
#include "Utilities.h"

using namespace Utilities;

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

        void options();
        void changeName(string name);
        void loggin(string sender,string message);
        void showlog();
        void lastSeen(string name);
};
