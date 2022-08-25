#include <math.h>
#include <stdio.h>
#include "crosstunnel.h"
#include "stdbool.h"

#define RGBA(r,g,b,a) (r << 24) + (g << 16) + (b << 8) + a
#define RGB(r,g,b) RGBA(r,g,b,255)

#define WIDTH 192
#define HEIGHT 192

// IDEAS
// things chasing around the cross and then smaller inset ones with offsets
//  the dot always moves in a direction for the same amount of pixels, maybe this is something? turtley vibe?
//  have multiple and offset them and it could look cool
//  also rainbows
// lemmings
// tempest thing

#define CROSSCOUNT 8

const double PI = M_PI;

const uint32_t BLACK = 255;
const uint32_t WHITE = RGB(255,255,255);
const uint32_t RED = RGB(255,0,0);

// clips a value to within the drawing area
int clip(int v) {
    if (v >= WIDTH || v >= HEIGHT) {
        return WIDTH-1;
    } else if (v <=0) {
        return 0;
    } else {
        return v;
    }
}

// parametric function for the x position on a cross. ranges from 0-1. used for drawing the rotating dots.
double crossx(double t) {
    double intpart;
    t = modf(t,&intpart);
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

double crossScales[CROSSCOUNT];

// parametric function for the y position on a cross. ranges from 0-1. used for drawing the rotating dots.
double crossy(double t) {
    return crossx(t+9/12.0);
}

// x and y are the point of the middle of the cross
void plotCross(uint32_t *pixels, float x, float y, float h, float bpos, float bsize, int color) {
    double t;
    for(double i=0;i<h*8;i++) {
        t = i/h/8;
        float c;
        if(t > bpos && t < bpos+bsize) {
            c = BLACK;
        } else {
            c = color;
        }
        pixels[\
        clip((int)(crossx(t)*h+x-(h/2))) \
        + clip((int)(crossy(t)*h+y-(h/2))) \
        *192] = c;
    }

}

void crosstunnel_init() {
    for(int i = 0; i<CROSSCOUNT; i++) {
        crossScales[i] = -i*(192/8);
    }

}


void crosstunnel_frame(uint32_t *pixels, uint32_t time) {
    double intpart;

    double t = time/5000.0;
    double scale = (time%2700/1000.0)+1.0;
    double pos = modf(t,&intpart);

    for(int p=0;p<192*192;p++)
        pixels[p] = 0;

    for(int i=0;i<CROSSCOUNT;i++) {
        if(i%2 == 1) {
         plotCross(pixels,96,96,crossScales[i],pos,0.05,RGB(0,i*32,0));
        } else {
         plotCross(pixels,96,96,crossScales[i],-pos+1,0.05,RGB(0,i*32,0));
        }
        crossScales[i] += 0.5; // TODO replace with time-based solution
        if(crossScales[i] > 192.0) {
            crossScales[i] = (192/8);
        }
    }
}
