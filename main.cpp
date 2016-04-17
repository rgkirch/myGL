                                  // main.cpp
// Richard Kirchofer
#include <stdio.h>
#include <math.h>
#include "draw.hpp"
#define NUM 100.0

int main(int argc, char** argv) {
    init(argc, argv);
    float* nums = (float*)malloc(sizeof(float)*8);
    nums[0] = 1.0/10; nums[1] = 1.0/10;
    nums[2] = 1.0/100; nums[3] = -1.0/100;
    nums[4] = -1.0/1000; nums[5] = -1.0/1000;
    nums[6] = -1.0/10000; nums[7] = 1.0/10000;
    //addPoint(8, nums);
    free(nums);
   // addPoint(-0.5, 0.0);
   // addPoint(0.5, 0.0);
   // addPoint(0.0, 0.5);
   // addPoint(0.0, -0.5);
    draw();
    return 0;
}
