#include <stdio.h>
#include <iostream>
#include "Bot.h"

using namespace programm;

int main(int argc,char **argv) {
    Bot *bot;
    if(argc!=4) {
        bot = new Bot();
    }
    else {
        bot = new Bot(argv[1],atoi(argv[2]),argv[3]);
    }
    delete bot;
    return 0;
}
