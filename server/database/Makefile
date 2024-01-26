LDFLAGS+=-pthread
CC=g++
OBJS=test.o ChatDataBase.o common.o DbPool.o
CPPFLAGS+=-Wall -g -std=c++11
MYSQLFLAG=`mysql_config --cflags --libs`
JSONFLAG=-ljsoncpp

test: $(OBJS)
	$(CC) $(OBJS) $(CPPFLAGS) $(LDFLAGS) $(MYSQLFLAG) $(JSONFLAG) -o test

test.o: test.cpp
	$(CC) test.cpp $(CPPFLAGS) -c -lpthread -o test.o

ChatDataBase.o: ChatDataBase.cpp
	$(CC) ChatDataBase.cpp $(CPPFLAGS) -c $(MYSQLFLAG) -o ChatDataBase.o  

common.o: ../../utility/common.cpp
	$(CC) ../../utility/common.cpp $(CPPFLAGS) -c -o common.o

DbPool.o: DbPool.cpp
	$(CC) DbPool.cpp $(CPPFLAGS) -c $(MYSQLFLAG) -o DbPool.o  

clean:
	rm -rf *.o test