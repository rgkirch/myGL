# Richard Kirchofer
CC=g++
#-I/usr/include -I/usr/local/include -L/usr/local/lib -L/usr/lib
#LINK=-lglfw -lGLEW -lGL -lpng -pthread
# -lboost_system -lboost_filesystem -lboost_thread -lboost_chrono -lboost_date_time 
STD=-std=c++14
LIBS=glfw3 opencv

INCLUDE:=-I/usr/include $(shell pkg-config --cflags $(LIBS)) $(shell Magick++-config --cppflags)
LINK:=$(shell Magick++-config --libs) $(shell pkg-config --libs $(LIBS)) -lboost_system -lboost_filesystem -pthread -fopenmp -lGL -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lGLEW -lXrandr -lXinerama  -lXxf86vm -lXcursor

all: main.cpp MyGL.cpp ../include/MyGL.hpp
	$(CC) -g -o app main.cpp MyGL.cpp -I../include $(STD) $(INCLUDE) $(LINK)
clean:
	rm app
