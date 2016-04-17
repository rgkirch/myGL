                                // draw.hpp
// Richard Kirchofer
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void init(int argc, char** argv);
void addPoint(float x, float y);
void addPoint(int length, float* nums);
void debug();
void draw();
