                                  // main.cpp
// Richard Kirchofer
#include <stdio.h>
#include <memory>
#include "MyGL.hpp"

int main(int argc, char** argv) {
    std::unique_ptr<MyGL> mygl;
    if(argc > 1) {
        std::make_unique<MyGL>(std::string(argv[1]));
    } else {
        std::make_unique<MyGL>();
    }
    mygl->collage();
    //if(argc > 1) boostFun(argv[1]);

    //delete mygl;
    //mygl->currentContext = new Context();
    //std::string data = ShaderProgram::readFile("vertexShader.glsl");
    //std::cout << data;
    return 0;
}
