# Richard Kirchofer
CC=g++
STUF=-I/usr/include -I/usr/local/include -L/usr/local/lib -L/usr/lib -lglfw -lGLEW -lGL -lpng -lboost_system -lboost_filesystem

STD=-std=c++11

all: app.exe
app.exe: main.cpp draw.o context.o
	$(CC) -g -o app.exe main.cpp draw.o context.o $(STD) $(STUF)
draw.o: draw.cpp
	$(CC) -g -c draw.cpp $(STD) $(STUF)
context.o: context.cpp
	$(CC) -g -c context.cpp $(STD) $(STUF)
clean:
	rm app.exe draw.o context.o
