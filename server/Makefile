LDFLAGS+=-pthread
CC=g++
OBJS=server.o conn.o ChatDataBase.o common.o DbPool.o
CPPFLAGS+=-Wall -g -std=c++11 -I ../utility -I database
MYSQLFLAG=`mysql_config --cflags --libs`
JSONFLAG=-ljsoncpp

server: $(OBJS)
	$(CC) $(OBJS) $(CPPFLAGS) $(LDFLAGS) $(MYSQLFLAG) $(JSONFLAG) -o server

server.o: server.cpp
	$(CC) server.cpp $(CPPFLAGS) -c -lpthread -o server.o

conn.o: conn.cpp
	$(CC) conn.cpp $(CPPFLAGS) -c -o conn.o

ChatDataBase.o: database/ChatDataBase.cpp
	$(CC) database/ChatDataBase.cpp $(CPPFLAGS) -c $(MYSQLFLAG) -o ChatDataBase.o  

common.o: ../utility/common.cpp
	$(CC) ../utility/common.cpp $(CPPFLAGS) -c -o common.o

DbPool.o: database/DbPool.cpp
	$(CC) database/DbPool.cpp $(CPPFLAGS) -c $(MYSQLFLAG) -o DbPool.o  

clean:
	rm -rf *.o server