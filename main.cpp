                                  // main.cpp
#include <stdio.h>
#include <math.h>
#include "plot.hpp"
#define NUM 100.0

int main() {
    init();
    for(int i = 0; i < NUM; ++i) {
        addPoint(i/NUM*2-1, sin(i/NUM*6-3));
    }
    draw();
    return 0;
}
