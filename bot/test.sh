g++ -c Bot.cpp;
g++ -c Connect.cpp;
g++ -c Database.cpp -lsqlite3;
g++ main.cpp -o main Connect.o Bot.o Database.o -lsqlite3;
./main irc.freenode.net 6667 NekkidxD;
