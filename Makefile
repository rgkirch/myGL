app.exe: plot.o
	g++ -g -o app.exe main.cpp plot.o -I/usr/include -I/usr/local/include -lglfw -lGLEW -lGLU -lGL
plot.o:
	g++ -g -c plot.cpp -I/usr/include -I/usr/local/include -lglfw -lGLEW -lGLU -lGL
clean:
	rm app.exe plot.o
