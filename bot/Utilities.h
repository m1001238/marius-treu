#include <stdio.h>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>

using namespace std;

namespace utilities {
    class InterpretedBuffer {
        public:
            string sender;
            vector<string> message;

            InterpretedBuffer(string buffer);
            int atPosition(string msgVar);
    };
    class TimeTools {
        public:
            static string getTimeStr(int timestamp);
    };
    class CharExtend {
        public:
            static char makeSmall(char c);
    };
}
