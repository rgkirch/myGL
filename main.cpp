                                  // main.cpp
// Richard Kirchofer
#include <stdio.h>
#include "draw.hpp"
#include "myGL.hpp"

int main(int argc, char** argv) {
    std::string data = ShaderProgram::readFile("vertexShader.glsl");
    std::cout << data;
    return 0;
}
