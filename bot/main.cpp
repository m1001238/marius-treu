#include <stdio.h>
#include <iostream>
#include "Bot.h"

int main(int argc,char **argv) {
    if(argc!=4) {
        perror("nicht genügend parameter");
        return 0;
    }

    Bot *bot = new Bot(argv[1],atoi(argv[2]),argv[3]);
    delete bot;
    return 0;
}
