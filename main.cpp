                                  // main.cpp
// Richard Kirchofer
#include <stdio.h>
#include "MyGL.hpp"

int main(int argc, char** argv) {
    MyGL* mygl = new MyGL();
    mygl->currentContext = new Context();
    //std::string data = ShaderProgram::readFile("vertexShader.glsl");
    //std::cout << data;
    return 0;
}
