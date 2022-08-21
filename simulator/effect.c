#include <math.h>
#include "effect.h"

void effect(uint32_t *pixels, uint32_t time) {
    double dt = ((double)time) / 499;

    for (int y = 0; y < 192; y++) {
        for (int x = 0; x < 192; x++) {
            double dx = (double)x;
            double dy = (double)y;
            double v = sin(dx/50+dt)+sin(dy/22+dt)+sin(dx/16);

            pixels[x + y * 192] = (((int)(128 + v * 64)) & 255) << 24;
        }
    }
}
