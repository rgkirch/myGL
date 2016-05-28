# Richard Kirchofer
CC=g++
#-I/usr/include -I/usr/local/include -L/usr/local/lib -L/usr/lib
#LINK=-lglfw -lGLEW -lGL -lpng -pthread
# -lboost_system -lboost_filesystem -lboost_thread -lboost_chrono -lboost_date_time 
STD=-std=c++14
PKG=pkg-config --print-errors
LIBS=glfw3 
#REQ=$(shell $(PKG) --print-requires --print-requires-private $(ORIG))
#LIBS= $(REQ) $(ORIG)


INCLUDE:=$(shell $(PKG) --cflags $(LIBS)) $(shell Magick++-config --cxxflags --cppflags)
#LINK=$(shell $(PKG) --libs $(ORIG)) -lboost_system -lboost_filesystem -pthread
LINK:=$(shell Magick++-config --ldflags --libs) -lboost_system -lboost_filesystem -pthread -fopenmp -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lGLEW -lXrandr -lXinerama  -lXxf86vm -lXcursor

all: main.cpp MyGL.cpp ../include/MyGL.hpp
	$(CC) -g -o app.exe main.cpp MyGL.cpp -I../include $(STD) $(INCLUDE) $(LINK)
clean:
	rm app.exe

#	for each in $(LIBS); do \
#		INCLUDE=$(INCLUDE) $($(PKG) --cflags $(each))
#		done
