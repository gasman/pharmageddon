#include <math.h>
#include "plasma.h"

void plasma_init(void) {
}

uint32_t plasma_pix(int x, int y, double dt) {
    double dx = (double)x;
    double dy = (double)y;
    double v = sin(dx/50+dt)+sin(dy/22+dt)+sin(dx/16);

    return (((int)(128 + v * 64)) & 255) << 24;
}

void plasma_frame(uint32_t *pixels, uint32_t time) {
    double dt = ((double)time) / 499;

    uint32_t *screen_pos = pixels + 64;
    for (int y = 0; y < 64; y++) {
        for (int x = 64; x < 128; x++) {
            *screen_pos = plasma_pix(x, y, dt);
            screen_pos++;
        }
        screen_pos += 128;
    }

    screen_pos = pixels + (64 * 192);
    for (int y = 64; y < 128; y++) {
        for (int x = 0; x < 192; x++) {
            *screen_pos = plasma_pix(x, y, dt);
            screen_pos++;
        }
    }

    screen_pos = pixels + (128 * 192) + 64;
    for (int y = 128; y < 192; y++) {
        for (int x = 64; x < 128; x++) {
            *screen_pos = plasma_pix(x, y, dt);
            screen_pos++;
        }
        screen_pos += 128;
    }
}
