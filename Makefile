all: app.exe
app.exe: main.cpp plot.o
	g++ -g -o app.exe main.cpp plot.o -std=c++11 -I/usr/include -I/usr/local/include -lglfw -lGLEW -lGLU -lGL
plot.o: plot.cpp
	g++ -g -c plot.cpp -std=c++11 -I/usr/include -I/usr/local/include -lglfw -lGLEW -lGLU -lGL
clean:
	rm app.exe plot.o
