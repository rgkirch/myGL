# Richard Kirchofer
CC=g++
all: app.exe
app.exe: main.cpp draw.o context.o
	$(CC) -g -o app.exe main.cpp draw.o context.o -std=c++11 -I/usr/include -I/usr/local/include -L/usr/local/lib -L/usr/lib -lglfw -lGLEW -lGL -lpng -lboost_system -lboost_filesystem
draw.o: draw.cpp
	$(CC) -g -c draw.cpp -std=c++11 -I/usr/include -I/usr/local/include -L/usr/local/lib -L/usr/lib -lglfw -lGLEW -lGL -lpng -lboost_system -lboost_filesystem
context.o: context.cpp
	$(CC) -g -c context.cpp -std=c++11 -I/usr/include -I/usr/local/include -L/usr/local/lib -L/usr/lib -lglfw -lGLEW -lGL -lpng -lboost_system -lboost_filesystem
clean:
	rm app.exe draw.o context.o
