                                  // main.cpp
#include <stdio.h>
#include "plot.hpp"

int main() {
    init();
    addPoint(0.0,0.0);
    debug();
    float points[] = {1,1, 0,1, 1,0};
    addPoint(3, points);
    draw();
    return 0;
}
