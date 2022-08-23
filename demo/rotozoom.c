#include <math.h>
#include "gfx.h"
#include "rotozoom.h"

gfx_image img;

void rotozoom_init(void) {
    gfx_loadimage("../assets/patarty.png", &img);
}

uint32_t rotozoom_pix(int x, int y, double a, double r) {
    double sx = ((double)x) - 96.0;
    double sy = ((double)y) - 96.0;

    int tx = (int)((sx * cos(a) + sy * sin(a)) * r) % img.width;
    if (tx < 0) tx += img.width;
    int ty = (int)((sy * cos(a) - sx * sin(a)) * r) % img.height;
    if (ty < 0) ty += img.height;

    unsigned char *pix = img.data + 4 * (ty * img.width + tx);
    return ((int)pix[0]) << 24 | ((int)pix[1]) << 16 | ((int)pix[2]) << 8;
}

void rotozoom_frame(uint32_t *pixels, uint32_t time) {
    double a = ((double)time) / 499.0;
    double r = 1.0 + 0.5 * sin((double) time / 400.0);

    uint32_t *screen_pos = pixels + 64;
    for (int y = 0; y < 64; y++) {
        for (int x = 64; x < 128; x++) {
            *screen_pos = rotozoom_pix(x, y, a, r);
            screen_pos++;
        }
        screen_pos += 128;
    }

    screen_pos = pixels + (64 * 192);
    for (int y = 64; y < 128; y++) {
        for (int x = 0; x < 192; x++) {
            *screen_pos = rotozoom_pix(x, y, a, r);
            screen_pos++;
        }
    }

    screen_pos = pixels + (128 * 192) + 64;
    for (int y = 128; y < 192; y++) {
        for (int x = 64; x < 128; x++) {
            *screen_pos = rotozoom_pix(x, y, a, r);
            screen_pos++;
        }
        screen_pos += 128;
    }
}
