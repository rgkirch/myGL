app.exe:
	g++ -g -o app.exe main.cpp plot.cpp -I/usr/include  -I/usr/local/include  -lglfw -lGLEW -lGLU -lGL
