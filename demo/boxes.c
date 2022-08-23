#include <math.h>
#include <stdlib.h>
#include "gfx.h"
#include "boxes.h"

gfx_image patarty;

void boxes_init(void) {
    gfx_loadimage("../assets/patarty.png", &patarty);
}

void boxes_frame(uint32_t *pixels, uint32_t time) {
    gfx_cls(pixels, 0x00110000);
    for (int y = 64; y < 128; y += 16) {
        for (int x = 0; x < 192; x += 16) {
            gfx_putpixel(pixels, 72 + x/4, y/4, 0x0000ff00);
            gfx_rect(pixels, x+1, y+1, 14, 14, (time + x + y & 255) << 16);
        }
    }
    int t = (time >> 4) % 64;
    gfx_filltri(pixels, 64, 191, 64 + t, 128, 127, 128+t, 0xff000000);
    gfx_fillcircle(pixels, 96, 96, t, 0x00ffff00);
    int patarty_y = -abs((int)(20.0 * sin(((double)time) / 100)));
    gfx_drawimage(pixels, &patarty, 96 - patarty.width / 2, 96 + patarty_y - patarty.height / 2);
}
