#define GLEW_STATIC
#include <GL/glew.h>

#include <string>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>

class ShaderProgram {
public:
    ShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName);
    static std::string readFile(std::string fileName);
    void checkShaderStepSuccess(GLuint shader, GLuint status);
    void printShaderLog(char *errorMessage, GLuint shader);
    GLuint id();
    GLuint program;
    GLint viewOffsetX;
    GLint viewOffsetY;
    GLint unitsPerPixelX;
    GLint unitsPerPixelY;
    GLint screenWidth;
    GLint screenHeight;
};

class ShaderTextured : ShaderProgram {
public:
};

