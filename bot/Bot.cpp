#include "Bot.h"

namespace programm {
    Config::Config() {
        char HOST[] = "";
        int PORT = 0;
        char CHANNEL[] = "";
        this->setConfigFile(HOST,PORT,CHANNEL);
    }
    Config::Config(char* HOST,int PORT,char* CHANNEL) {
        this->setConfigFile(HOST,PORT,CHANNEL);
    }
    Config::~Config() {
        delete this->configData;
    }
    void Config::setConfigFile(char* HOST,int PORT,char* CHANNEL) {
        const char* FILENAME = "config.cfg";
        ofstream outfile;
        ifstream infile;

        //Load config
        this->configData = new ConfigData();
        infile.open(FILENAME);
        infile.read((char*)this->configData, sizeof(ConfigData));
        infile.close();

        cout << this->configData->HOST << " -> " << this->configData->PORT << " -> " << this->configData->CHANNEL << "\n" << endl;

        if(this->configData->PORT==0 && PORT==0) {
            strcpy(this->configData->HOST , "irc.freenode.net");
            this->configData->PORT = 6667;
            strcpy(this->configData->CHANNEL , "NekkidxD");
        }
        else if(PORT!=0) {
            strcpy(this->configData->HOST , HOST);
            this->configData->PORT = PORT;
            strcpy(this->configData->CHANNEL , CHANNEL);
        }

        //Save config
        outfile.open(FILENAME, ofstream::trunc);
        outfile.write((char*)this->configData, sizeof(ConfigData));
        outfile.close();

        cout << this->configData->HOST << " -> " << this->configData->PORT << " -> " << this->configData->CHANNEL << "\n" << endl;
    }

    Bot::Bot() {
        this->config = new Config();
        this->run();
    }
    Bot::Bot(char *HOST,int PORT,char *CHANNEL) {
        this->config = new Config(HOST,PORT,CHANNEL);
        this->run();
    }
    void Bot::run() {
        this->enableLogging=false;
        this->db = new Database();

        this->connect = new Connect(this->config->configData->HOST,this->config->configData->PORT,this->config->configData->CHANNEL);

        while(true) {
            if (this->irc_parse(this->connect->run()) == -1)
                break;
        }

        delete this->connect;
    }
    Bot::~Bot() {
        delete this->connect;
        delete this->config;
        delete this->db;
    }

    int Bot::irc_parse(string buffer) {
        if (buffer.find("\r\n") == buffer.length()-2) buffer.erase(buffer.length()-2);
        this->connect->ping_parse(buffer);
        return this->interpret_parse(buffer);
    }

    int Bot::interpret_parse(string buffer) {
        InterpretedBuffer data(buffer);
        //if(data.sender=="") return 0;
        int pos=0;

        if(this->enableLogging) {
            stringstream msg;
            for(int i=0;i<(int)data.message.size();i++)
                msg << data.message[i] << " ";
            this->loggin(data.sender,msg.str());
        }

        if( (pos=data.atPosition("-o")) != -1 ) {
            this->options();
        }
        else if( (pos=data.atPosition("-log")) != -1 && (int)data.message.size() > pos+1 ) {
            if( data.message[pos+1] == "on" ) {
                this->enableLogging=true;
                this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :Loggin enabled\r\n").c_str());
            }
            else if( data.message[pos+1] == "off" ) {
                this->enableLogging=false;
                this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :Loggin disabled\r\n").c_str());
            }
        }
        else if( (pos=data.atPosition("-showlog")) != -1 )
            this->showlog();
        else if( (pos=data.atPosition("-last")) != -1 && data.message.size() > pos+1 )
            this->lastSeen(data.message[pos+1]);
        else if( (pos=data.atPosition("-shutdown")) != -1 ) {
            this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :bye bye!\r\n").c_str());
            return -1;
        }
        else if( (pos=data.atPosition("-topic")) != -1 && (int)data.message.size() > pos+2 )
            this->changeTopic( data.message[pos+1] , data.message[pos+2] );
        else if( (pos=data.atPosition("-cn")) != -1 && (int)data.message.size() > pos+1 ) {
            this->changeName( data.message[pos+1] );
        }
        else if( (pos=data.atPosition("-join")) != -1 && (int)data.message.size() > pos+1 )
            this->joinChannel( data.message[pos+1] );
        else if( (pos=data.atPosition("-leave")) != -1 && (int)data.message.size() > pos+1 )
            this->leaveChannel( data.message[pos+1] );
        else if( (pos=data.atPosition("-curtime")) != -1 )
            this->getCurTime();
        else if( (pos=data.atPosition("-love")) != -1 && (int)data.message.size() > pos+2 )
            this->love( data.message[pos+1] , data.message[pos+2] );

        return 0;
    }

    /*
    Bot-functions
    */

    void Bot::options() {
        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :Du kannst folgendes tun:\r\n").c_str());
        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-cn name (ändert meinen namen)\r\n").c_str());
        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-log on/off (schaltet logging ein/aus)\r\n").c_str());
        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-showlog (um die letzten 10 logs auszuzeigen - nach Zeit sortiert)\r\n").c_str());
        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-last name (um anzuzeigen wann der angegebene name zuletzt etwas getan hat)\r\n").c_str());
        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-join channel (um einen anderen channel zu joinen - ohne '#')\r\n").c_str());
        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-leave channel (um einen anderen channel zu leaven - ohne '#')\r\n").c_str());
        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-topic channel topic (um ein channel-topic umzubenennen - ohne '#')\r\n").c_str());
        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-curtime (um die momentane Zeit zu bekommen)\r\n").c_str());
        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-love (um zu prüfen ob du und dein Partner zusammen passen - cooles feature ;-) )\r\n").c_str());
        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-shutdown (um den bot auszumachen)\r\n").c_str());
    }

    void Bot::loggin(string sender, string message) {
        this->db->insertIntoDB(sender,message);
    }

    void Bot::showlog() {
        string output = this->db->showAllLoggin();
        size_t pos = 0;
        while( (pos = output.find_first_of('\n')) != string::npos ) {
            string substr = output.substr(0,pos);
            this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :"+substr.c_str()+"\r\n").c_str());
            output = output.substr(pos+1,output.length());
        }
    }

    void Bot::lastSeen(string name) {
        string output = this->db->selectLastSeen(name);
        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :"+output.c_str()+"\r\n").c_str());
    }
    void Bot::changeName(string name) {
        this->connect->setBotname(name);
        this->connect->s2u(("NICK " + this->connect->getBotname() + "\r\n").c_str());
    }
    void Bot::joinChannel(string chan) {
        this->connect->s2u(("JOIN #" + chan + "\r\n").c_str());
    }
    void Bot::leaveChannel(string chan) {
        this->connect->s2u(("PART #" + chan + "\r\n").c_str());
    }
    void Bot::changeTopic(string chan, string topic) {
        this->connect->s2u(("TOPIC #" + chan + " " + topic + "\r\n").c_str());
    }
    void Bot::getCurTime() {
        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :Es ist: "+ utilities::TimeTools::getTimeStr( time(0) ) +"\r\n").c_str());
    }
    void Bot::love(string du,string partner) {
        float word1=0.0f;
        float word2=0.0f;
        int wordCount1=0;
        int wordCount2=0;

        for(int i=0;i<du.length();i++) {
            char c = utilities::CharExtend::makeSmall( du.at(i) );
            if(c!='\0') {
                word1 += (float)( (int)c-97 ) / (float)26;
                wordCount1++;
            }
        }
        for(int i=0;i<partner.length();i++) {
            char c = utilities::CharExtend::makeSmall( partner.at(i) );
            if(c!='\0') {
                word2 += (float)( (int)c-97 ) / (float)26;
                wordCount2++;
            }
        }
        float p1 = ( word1/wordCount1 );
        float p2 = ( word2/wordCount2 );
        float percent = 0.0f;

        if(p1>p2)
            percent = p1-p2;
        else if(p2>p1)
            percent = p2-p1;

        stringstream sstr;
        sstr << (int)( (1.0f-percent)*100.0f );

        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :Du und dein Partner passen zu "+ sstr.str().c_str() +"% zusammen!!!\r\n").c_str());
    }
}
