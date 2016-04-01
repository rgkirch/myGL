                                // visualize.hpp
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// draw a dot at the given coordinates
void dot(float x, float y);
// do init stuf
void init(GLFWwindow* &theWindow, char* nameOfWindow, GLuint windowWidth, GLuint windowHeight);
// allocate memory, read a file into that memory, and return that memory
GLchar* readFile(char* fileName);
// creates a glShader program from files
GLuint createShader(char* vertexShaderFileName, char* fragmentShaderFileName);
void printShaderLog(char* errorMessageWithoutNewline, GLuint shaderProgram);
void checkShaderStepSuccess(GLint shaderProgramID, GLuint statusToCheck);
void windowSize(GLFWwindow*, int width, int height);

unsigned int screenWidth;
unsigned int screenHeight;
int initCalled = 0;
