                                  // main.cpp
#include <stdio.h>
#include <math.h>
#include "plot.hpp"
#define NUM 100.0

int main() {
    init();
    addPoint(-0.5, 0.0);
    addPoint(0.5, 0.0);
    addPoint(0.0, 0.5);
    addPoint(0.0, -0.5);
    draw();
    return 0;
}
