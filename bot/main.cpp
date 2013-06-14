#include <stdio.h>
#include <iostream>
#include "Bot.h"

using namespace programm;

int main(int argc,char **argv) {
    Bot *bot;
    if(argc==1) {
        bot = new Bot();
    }
    else if(argc==4) {
        bot = new Bot(argv[1],atoi(argv[2]),argv[3]);
    }
    else {
        printf("Unzulaessige parameter");
    }
    delete bot;
    return 0;
}
