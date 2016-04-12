# Richard Kirchofer
CC=g++
all: app.exe
app.exe: main.cpp plot.o
	$(CC) -g -o app.exe main.cpp plot.o -std=c++11 -I/usr/include -I/usr/local/include -lglfw -lGLEW -lGL
plot.o: plot.cpp
	$(CC) -g -c plot.cpp -std=c++11 -I/usr/include -I/usr/local/include -lglfw -lGLEW -lGL
clean:
	rm app.exe plot.o
