#include <stdlib.h>
#include "gfx.h"

#define WIDTH 192
#define HEIGHT 192

void gfx_cls(uint32_t *pixels, uint32_t colour) {
    uint32_t *end = pixels + (WIDTH * HEIGHT);
    for (uint32_t *i = pixels; i < end; i++) {
        *i = colour;
    }
}

void gfx_putpixel(uint32_t *pixels, int x, int y, uint32_t colour) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
      pixels[(y * WIDTH) + x] = colour;
}

void gfx_fillrect(uint32_t *pixels, int x1, int y1, int x2, int y2, uint32_t colour) {
    if (x1 < 0) x1 = 0;
    if (x2 <= x1) return;
    if (y1 < 0) y1 = 0;
    if (y2 <= y1) return;
    for (int y = y1; y < y2; y++) {
        if (y >= HEIGHT) return;
        uint32_t *i = pixels + (y * WIDTH) + x1;
        for (int x = x1; x < x2; x++) {
            if (x >= WIDTH) break;
            *i = colour;
            i++;
        }
    }
}

void gfx_line(uint32_t *pixels, int x1, int y1, int x2, int y2, uint32_t colour) {
    int x, y, d, deltax, deltay;
    int dinc1, dinc2, xinc1, xinc2, yinc1, yinc2;
    int i, numpixels;

    deltax = abs(x2-x1);
    deltay = abs(y2-y1);
    if (deltax >= deltay) {
        numpixels = deltax+1;
        d = (deltay << 1) - deltax;
        dinc1 = deltay << 1;
        dinc2 = (deltay - deltax) << 1;
        xinc1 = 1; xinc2 = 1;
        yinc1 = 0; yinc2 = 1;
    } else {
        numpixels = deltay+1;
        d = (deltax << 1) - deltay;
        dinc1 = deltax << 1;
        dinc2 = (deltax - deltay) << 1;
        xinc1 = 0; xinc2 = 1;
        yinc1 = 1; yinc2 = 1;
    }
    if (x1 > x2) {xinc1 = -xinc1; xinc2 = -xinc2;}
    if (y1 > y2) {yinc1 = -yinc1; yinc2 = -yinc2;}

    // Start drawing
    x=x1; y=y1;
    for (i=0; i < numpixels; i++) {
        gfx_putpixel(pixels, x, y, colour);
        if (d < 0) {
            d += dinc1;
            x += xinc1;
            y += yinc1;
        } else {
            d += dinc2;
            x += xinc2;
            y += yinc2;
        }
    }
}
