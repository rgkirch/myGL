# Richard Kirchofer
CC=g++
STUF=-I/usr/include -I/usr/local/include -L/usr/local/lib -L/usr/lib -lglfw -lGLEW -lGL -lpng -lboost_system -lboost_filesystem

STD=-std=c++11

all: app.exe
app.exe: main.cpp MyGL.o
	$(CC) -g -o app.exe main.cpp MyGL.o $(STD) $(STUF)
MyGL.o: MyGL.cpp
	$(CC) -g -c MyGL.cpp $(STD) $(STUF)
clean:
	rm app.exe draw.o MyGL.o
