g++ -c Bot.cpp;
g++ -c Connect.cpp;
g++ -c Database.cpp -lsqlite3;
g++ -c Utilities.cpp;
g++ main.cpp -o main Connect.o Bot.o Database.o Utilities.o -lsqlite3;
./main irc.freenode.net 6667 NekkidxD;
