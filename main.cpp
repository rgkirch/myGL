                                  // main.cpp
// Richard Kirchofer
#include <stdio.h>
#include <memory>
#include "MyGL.hpp"

int main(int argc, char** argv) {
    std::unique_ptr<MyGL>mygl(new MyGL());
    //delete mygl;
    //mygl->currentContext = new Context();
    //std::string data = ShaderProgram::readFile("vertexShader.glsl");
    //std::cout << data;
    return 0;
}
