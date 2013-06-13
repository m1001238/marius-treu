#include "Bot.h"

Bot::Bot(char *HOST,int PORT,char *CHANNEL) {
    this->enableLogging=false;
    this->db = new Database();

    this->connect = new Connect(HOST,PORT,CHANNEL);

    while(true) {
        if (this->irc_parse(this->connect->run()) == -1)
            break;
    }

    delete this->connect;
}
Bot::~Bot() {
    delete this->connect;
}

int Bot::irc_parse(string buffer) {
    if (buffer.find("\r\n") == buffer.length()-2) buffer.erase(buffer.length()-2);
    this->connect->ping_parse(buffer);
    return this->interpret_parse(buffer);
}

int Bot::interpret_parse(string buffer) {
    Utilities::InterpretedBuffer data(buffer);
    //if(data.sender=="") return 0;
    int pos=0;

    if(this->enableLogging) {
        stringstream msg;
        for(int i=0;i<(int)data.message.size();i++)
            msg << data.message[i];
        this->loggin(data.sender,msg.str());
    }

    if( (pos=data.atPosition("-o")) != -1 ) {
        this->options();
    }
    else if( (pos=data.atPosition("-cn")) != -1 && (int)data.message.size() > pos+1 ) {
        this->changeName( data.message[pos+1] );
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
    else if( (pos=data.atPosition("-shutdown")) != -1 )
        return -1;

    return 0;
}

//Bot-functions

void Bot::options() {
    this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :Du kannst folgendes tun:\r\n").c_str());
    this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-cn name (ändert meinen namen)\r\n").c_str());
    this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-log on/off (schaltet logging ein/aus)\r\n").c_str());
    this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-showlog (um die letzten 10 logs auszuzeigen - nach Zeit sortiert)\r\n").c_str());
    this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-last name (um anzuzeigen wann der angegebene name zuletzt etwas getan hat)\r\n").c_str());
    this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-shutodown (um den bot aus zu machen)\r\n").c_str());
}

void Bot::changeName(string name) {
    this->connect->setBotname(name);
    this->connect->s2u(("NICK " + this->connect->getBotname() + "\r\n").c_str());
}

void Bot::loggin(string sender, string message) {
    this->db->insertIntoDB(sender,message);
    //this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :ich logge mit -> "+sender+" : "+message+"\r\n").c_str());
}

void Bot::showlog() {
    string output = this->db->showAllLoggin();
    size_t pos = 0;
    while( (pos = output.find_first_of('\n')) != string::npos ) {
        string substr = output.substr(0,pos);
        printf("%s\n",substr.c_str());
        this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :"+substr.c_str()+"\r\n").c_str());
        output = output.substr(pos+1,output.length());
    }
}

void Bot::lastSeen(string name) {
    string output = this->db->selectLastSeen(name);
    this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :"+output.c_str()+"\r\n").c_str());
}
