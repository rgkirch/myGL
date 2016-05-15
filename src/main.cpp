                                  // main.cpp
// Richard Kirchofer
#include <stdio.h>
#include <memory>
#include "MyGL.hpp"

int main(int argc, char** argv) {
    //std::unique_ptr<MyGL>mygl(new MyGL());
    MyGL* mygl = new MyGL();
    mygl->mainLoop();

    printf("main done\n");
    //delete mygl;
    //mygl->currentContext = new Context();
    //std::string data = ShaderProgram::readFile("vertexShader.glsl");
    //std::cout << data;
    return 0;
}
