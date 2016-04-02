                                  // main.cpp
#include <stdio.h>
#include "plot.hpp"

int main() {
    float nums[2] = {0,0};
    printf("start\n");
    init();
    addPoint(1, nums);
    //float points[] = {1,1, 0,1, 1,0};
    //addPoint(3, points);
    draw();
    printf("finish\n");
    return 0;
}
