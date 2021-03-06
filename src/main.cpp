                                  // main.cpp
// Richard Kirchofer
#include <stdio.h>
#include <memory>
#include <boost/program_options.hpp>
#include "MyGL.hpp"

int main(int argc, char* argv[]) {
    //boost::program_options::option_description desc;
    std::unique_ptr<MyGL> mygl;
    if(argc > 1) {
        mygl = std::make_unique<MyGL>(std::string(argv[1]));
        mygl->cubeCollage(argv[1]);
    } else {
        mygl = std::make_unique<MyGL>();
        mygl->collage("");
    }
    //if(argc > 1) boostFun(argv[1]);

    //delete mygl;
    //mygl->currentContext = new Context();
    //std::string data = ShaderProgram::readFile("vertexShader.glsl");
    //std::cout << data;
    return 0;
}
