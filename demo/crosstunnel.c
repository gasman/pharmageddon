#include <math.h>
#include "crosstunnel.h"
#include "gfx.h"

#define CROSSCOUNT 8

// parametric function for the x position on a cross. ranges from 0-1. used for drawing the rotating dots.
double crossx(double t) {
    t = fmod(t, 1.0);
    if(t<(1/12.0)) {
        return t*4;
    } else if(t<(2/12.0)) {
        return (double)1/3.0;
    } else if (t<(3/12.0)) {
        return t*4-(1/3.0);
    } else if (t<(4/12.0)) {
        return 2/3.0;
    } else if (t<(5/12.0)) {
        return t*4-(2/3.0);
    } else if (t<(6/12.0)) {
        return 1;
    } else if (t<(7/12.0)) {
        return -(t*4)+3.0;
    } else if (t<(8/12.0)) {
        return 2/3.0;
    } else if (t<(9/12.0)) {
        return -(t*4)+3.0+(1/3.0);
    } else if (t<(10/12.0)) {
        return 1/3.0;
    } else if (t<(11/12.0)) {
        return -(t*4)+3.0+(2/3.0);
    } else {
        return 0;
    }
}

// parametric function for the y position on a cross. ranges from 0-1. used for drawing the rotating dots.
double crossy(double t) {
    return crossx(t+9/12.0);
}

// x and y are the point of the middle of the cross
void plotCross(uint32_t *pixels, float x, float y, float h, float bpos, float bsize, int color) {
    double t;
    for(double i=0;i<h*8;i++) {
        t = i/h/8;
        if(t > bpos && t < bpos+bsize) {
            // leave black
        } else {
            gfx_putpixel(pixels, (int)(crossx(t)*h+x-(h/2)), (int)(crossy(t)*h+y-(h/2)), color);
        }
    }

}

void crosstunnel_init() {
}


void crosstunnel_frame(uint32_t *pixels, uint32_t time) {
    double pos = fmod(((double)time)/5000.0, 1.0);

    gfx_cls(pixels, 0x00000000);

    for(int i=0;i<CROSSCOUNT;i++) {
        double scale = fmod((-i*(192/8)) + ((double)time)/5, 192.0);
        if(i%2 == 1) {
         plotCross(pixels,96,96,scale,pos,0.05,i*32<<16);
        } else {
         plotCross(pixels,96,96,scale,-pos+1,0.05,i*32<<16);
        }
    }
}
