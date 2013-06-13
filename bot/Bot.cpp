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
    string sender = "";
    string message = "";

    //if(this->enableLogging) this->logging(buffer);

    //sender
    size_t dd=0;
    size_t ex=0;
    size_t at=0;
    dd = buffer.find_first_of(':');
    ex = buffer.find_first_of('!');
    at = buffer.find_first_of('@');

    if(dd<ex && ex<at)
        sender = buffer.substr( (int)dd+1 , (int)ex-(int)dd-1 );

    size_t pos = 0;
    //message
    if ((pos = buffer.find("PRIVMSG")) != buffer.npos) {
        string sstr = buffer.substr( (int)pos , buffer.length()-(int)pos );
        if ((pos = sstr.find_first_of(':')) != sstr.npos) {
            message = sstr.substr( (int)pos+1 , sstr.length()-(int)pos );
        }
    }
    //logging
    if(this->enableLogging)
        this->loggin(sender,message);
    //options
    if ((pos = message.find(this->connect->getBotname()+":")) != buffer.npos) {
        int len = this->connect->getBotname().length()+2;
        return this->bot_functions(sender,message.substr( (int)pos+len , message.length()-((int)pos+len) ));
    }
    return 0;
}

int Bot::bot_functions(string sender, string message) {
    //this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :"+buffer+"\r\n").c_str());
    size_t pos = 0;
    int len = 0;
    if ((pos = message.find("-o")) != message.npos) {
        return this->options();
    }
    else if ((pos = message.find("-cn")) != message.npos) {
        len = 4;
        return this->changeName(message.substr( (int)pos+len , message.length()-((int)pos+len) ));
    }
    else if ((pos = message.find("-log")) != message.npos) {
        len = 5;
        string option = message.substr( (int)pos+len , message.length()-((int)pos+len) );
        if(option.find("on") != string::npos) {
            this->enableLogging=true;
            this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :Loggin enabled\r\n").c_str());
        }
        else if(option.find("off") != string::npos) {
            this->enableLogging=false;
            this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :Loggin disabled\r\n").c_str());
        }
        return 0;
    }
    else if ((pos = message.find("-showlog")) != message.npos) {
        this->showlog();
    }

    //printf("\n||||||%s|||||||||",buffer.c_str());
    return 0;
}

int Bot::options() {
    this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :Du kannst folgendes tun:\r\n").c_str());
    this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-cd name (ändert meinen namen)\r\n").c_str());
    this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-log on/off (schaltet logging ein/aus)\r\n").c_str());
    this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :-showlog (um die letzten 20 logs auszugeben)\r\n").c_str());
    return 0;
}

int Bot::changeName(string name) {
    this->connect->setBotname(name);
    this->connect->s2u(("NICK " + this->connect->getBotname() + "\r\n").c_str());
    return 0;
}

void Bot::loggin(string sender, string message) {
    this->db->insertIntoDB(sender,message);
    //this->connect->s2u(("PRIVMSG #" + this->connect->getChannel() + " :ich logge mit -> "+sender+" : "+message+"\r\n").c_str());
}

void Bot::showlog() {

}
