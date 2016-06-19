#include <iostream>
#include <cstdio>
#include <Magick++.h>

void readAll(std::string filename) {
    std::list<Magick::Image> images;
    Magick::readImages(&images, filename);
    Magick::animateImages(images.begin(), images.end());
}

void show(std::string filename) {
    Magick::Image image;
    image.read(filename + "[99999]");
    image.display();
}

int main(int argc, char* argv[]) {
    if(argc > 1) {
        for(int i = 1; i < argc; ++i) {
            show(std::string(argv[i]));
        }
    } else {
        std::cout << "pass at least one argument" << std::endl;
    }
}
