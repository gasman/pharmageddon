#include <math.h>
#include <stdlib.h>
#include "gfx.h"
#include "patarty.h"

gfx_image patarty;
gfx_image lavalamp;

void patarty_init(void) {
    gfx_loadimage("../assets/patarty.png", &patarty);
    gfx_loadimage("../assets/lavalamp.png", &lavalamp);
}

static uint32_t lava_pix(int x, int y, double dt) {
    int yr = (y-(int)(dt/15)) & 511;
    int yg = (y-(int)(dt/30)) & 511;
    int yb = (y-(int)(dt/45)) & 511;
    return (
        (lavalamp.data[(yr*lavalamp.width+x)*4] & 0x80) << 24
        | (lavalamp.data[(yg*lavalamp.width+x+128)*4] & 0x80) << 16
        | (lavalamp.data[(yb*lavalamp.width+x+256)*4] & 0x80) << 8
    );
}

void patarty_frame(uint32_t *pixels, uint32_t time) {
    double dt = ((double)time);

    uint32_t *screen_pos = pixels + 64;
    for (int y = 0; y < 64; y++) {
        for (int x = 64; x < 128; x++) {
            *screen_pos = lava_pix(x, y, dt);
            screen_pos++;
        }
        screen_pos += 128;
    }

    screen_pos = pixels + (64 * 192);
    for (int y = 64; y < 128; y++) {
        for (int x = 0; x < 192; x++) {
            *screen_pos = lava_pix(x, y, dt);
            screen_pos++;
        }
    }

    screen_pos = pixels + (128 * 192) + 64;
    for (int y = 128; y < 192; y++) {
        for (int x = 64; x < 128; x++) {
            *screen_pos = lava_pix(x, y, dt);
            screen_pos++;
        }
        screen_pos += 128;
    }

    int patarty_y = -abs((int)(20.0 * sin(((double)time) / 109)));
    gfx_drawimage(pixels, &patarty, 96 - patarty.width / 2, 96 + patarty_y - patarty.height / 2);
}
