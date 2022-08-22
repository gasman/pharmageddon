#include <math.h>
#include "gfx.h"
#include "boxes.h"

void boxes_init(void) {
}

void boxes_frame(uint32_t *pixels, uint32_t time) {
    gfx_cls(pixels, 0x00110000);
    for (int y = 64; y < 128; y += 16) {
        for (int x = 0; x < 192; x += 16) {
            gfx_fillrect(pixels, x+2, y+2, x+15, y+15, (time + x + y & 255) << 16);
        }
    }
}
