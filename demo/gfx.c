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

void gfx_fillrect(uint32_t *pixels, int x, int y, int w, int h, uint32_t colour) {
    if (x < 0) x = 0;
    if (w <= 0) return;
    if (y < 0) y = 0;
    if (h <= 0) return;
    int x_end = x + w;
    if (x_end > WIDTH) x_end = WIDTH;
    int y_end = y + h;
    if (y_end > HEIGHT) y_end = HEIGHT;
    for (int cy = y; cy < y_end; cy++) {
        uint32_t *i = pixels + (cy * WIDTH) + x;
        for (int cx = x; cx < x_end; cx++) {
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

void gfx_rect(uint32_t *pixels, int x, int y, int w, int h, uint32_t colour) {
    int x1, y1, x2, y2;
    x1 = x; y1 = y;
    x2 = x + w - 1; y2 = y + h - 1;

    if (x2 >= 0 && y2 >= 0 && x1 < WIDTH && y1 < HEIGHT) {
        gfx_line(pixels, x1, y1, x2, y1, colour);
        gfx_line(pixels, x1, y2, x2, y2, colour);
        gfx_line(pixels, x1, y1, x1, y2, colour);
        gfx_line(pixels, x2, y1, x2, y2, colour);
    }
}
