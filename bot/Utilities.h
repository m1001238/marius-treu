#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

namespace Utilities {
    class InterpretedBuffer {
        public:
            string sender;
            vector<string> message;

            InterpretedBuffer(string buffer);
            int atPosition(string msgVar);
    };
}
/*
int main() {
    //Utilities::InterpretedBuffer buffer = Utilities::interpretBuffer( string(":ficker!d9e51dc0@gateway/web/freenode/ip.217.229.29.192 JOIN #NekkidxD") );
    //Utilities::InterpretedBuffer buffer = Utilities::interpretBuffer( string(":ficker!d9e51dc0@gateway/web/freenode/ip.217.229.29.192 PRIVMSG #NekkidxD :hallo") );
    Utilities::InterpretedBuffer buffer = Utilities::interpretBuffer( string(":ficker!d9e51dc0@gateway/web/freenode/ip.217.229.29.192 PRIVMSG #NekkidxD :NekkidBot: -o hallo") );


    for(int i=0;i<(int)buffer.message.size();i++) {
        printf(">>%s<<\n",buffer.message[i].c_str());

    }
    string test = "NekkidBot";
    printf("%i",Utilities::atPosition(buffer,":"+test+":") );
    return 0;
}
*/
