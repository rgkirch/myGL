#include <iostream>
#include <iomanip>
#include <cstdio>
#include <Magick++.h>

int main(int argc, char* argv[]) {
    if(argc > 1) {
        Magick::Image image(argv[1]);
        std::vector<std::pair<Magick::Color,unsigned long> > histogram;
        Magick::colorHistogram( &histogram, image );
        std::vector<std::pair<Magick::Color,unsigned long> >::const_iterator p=histogram.begin();
        while (p != histogram.end()) {
            std::cout << (int)p->second << ": ("
                 << std::setw(3) << (int)p->first.redQuantum() << ","
                 << std::setw(3) << (int)p->first.greenQuantum() << ","
                 << std::setw(3) << (int)p->first.blueQuantum() << ")"
                 << std::endl;
            p++;
        }
    } else {
        std::cout << "pass at least one argument" << std::endl;
    }
}
