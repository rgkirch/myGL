# Richard Kirchofer
CC=g++
INCLUDE=-I/usr/include -I/usr/local/include -L/usr/local/lib -L/usr/lib
LINK=-lglfw -lGLEW -lGL -lpng -lboost_system -lboost_filesystem -lboost_thread -lboost_chrono -lboost_date_time

STD=-std=c++11

all: app.exe
app.exe: main.cpp MyGL.o
	$(CC) -g -o app.exe main.cpp MyGL.o $(STD) $(INCLUDE) $(LINK)
MyGL.o: MyGL.cpp
	$(CC) -g -c MyGL.cpp $(STD) $(INCLUDE) $(LINK)
clean:
	rm app.exe MyGL.o
