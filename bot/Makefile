all:
	g++ -c Bot.cpp
	g++ -c Connect.cpp
	g++ -c Database.cpp
	g++ -c Utilities.cpp
	g++ main.cpp -o main Connect.o Bot.o Database.o Utilities.o -lsqlite3
