                                   // plot.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "plot.h"

void plot(int length, float* xs, float* ys) {
    // look at all of the numbers, save value of largest and smallest on x and y
    // call a drawPoint() function in visualize.cpp at correct location in range of [-1, 1]
    // visualize knows window resolution, visualize will draw dot
    int xmax, ymax, xmin, ymin;
    xmax = ymax = xmin = ymin = 0;
    int x, y;
    x = y = 0;
    // performance improvment (memory): loop through x before y
    for( int i=0; i<length; ++i ) {
        x = xs[i];
        y = ys[i];
        if( x > xmax ) {
            xmax = x;
        } else if( x < xmin ) {
            xmin = x;
        }
        if( y > ymax ) {
            ymax = y;
        } else if( y < ymin ) {
            ymin = y;
        }
    }
}
