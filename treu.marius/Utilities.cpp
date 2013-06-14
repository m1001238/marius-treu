#include "Utilities.h"

namespace utilities {
    char CharExtend::makeSmall(char c) {
        if( (int)c>=(int)'A' && (int)c<=(int)'Z' ) {
            return (char)((int)c + 32);
        }
        else if( (int)c>=(int)'a' && (int)c<=(int)'z' ) {
            return c;
        }
        return '\0';
    }

    string TimeTools::getTimeStr(int timestamp) {
        time_t test;
        struct tm * timeinfo;
        test = timestamp;
        timeinfo = localtime ( &test );
        /// Zugriff auf einzelne Elemente
        stringstream stream;
        stream << "german time:" << timeinfo->tm_mday << "/" << timeinfo->tm_mon << "/" << 1900+timeinfo->tm_year << " h:" << timeinfo->tm_hour << " m:" << timeinfo->tm_min << " s:" << timeinfo->tm_sec;
        return stream.str();
    }

    InterpretedBuffer::InterpretedBuffer(string buffer) {
        this->sender="";
        /*
            MESSAGE
        */
        this->sender = "";
        size_t dd=0;
        size_t ex=0;
        size_t at=0;
        dd = buffer.find_first_of(':');
        ex = buffer.find_first_of('!');
        at = buffer.find_first_of('@');

        if(dd<ex && ex<at)
            this->sender = buffer.substr( (int)dd+1 , (int)ex-(int)dd-1 );

        /*
            MESSAGE
        */
        string space(" ");
        size_t pos=0;

        //starting space finding
        if( ( pos = buffer.find(space) ) != string::npos )
            buffer = buffer.substr( pos+1 , buffer.length()-(int)pos-1 );

        while( buffer.length() > 0 ) {
            //next space or finish finding
            if( ( pos = buffer.find(space) ) != string::npos) {
                this->message.push_back( buffer.substr( 0 , (int)pos ) );
                buffer = buffer.substr( (int)pos+1 , (int)buffer.length()-(int)pos-1 );
            }
            else {
                this->message.push_back( buffer = buffer.substr( 0 , (int)buffer.length() ) );
                break;
            }
        }
    }
    int InterpretedBuffer::atPosition(string msgVar) {
        for(int i=0;i<this->message.size();i++)
            if(this->message[i]==msgVar) return i;
        return -1;
    }
}
