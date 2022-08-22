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
