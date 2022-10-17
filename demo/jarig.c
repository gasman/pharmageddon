#include <stdlib.h>
#include "gfx.h"
#include "jarig.h"

gfx_image jarig1, jarig2, hetedansactie1, hetedansactie2;

void jarig_init(void) {
    gfx_loadimage("../assets/jarig1.png", &jarig1);
    gfx_loadimage("../assets/jarig2.png", &jarig2);
    gfx_loadimage("../assets/hetedansactie1.png", &hetedansactie1);
    gfx_loadimage("../assets/hetedansactie2.png", &hetedansactie2);
}

void jarig_frame(uint32_t *pixels, uint32_t time) {
    gfx_cls(pixels, 0x00000000);
    if (time % 700 < 350) {
        gfx_drawimage(pixels, &jarig1, 96 - jarig1.width / 2, 64);
    } else {
        gfx_drawimage(pixels, &jarig2, 96 - jarig2.width / 2, 64);
    }
    if (time % 350 < 175) {
        gfx_drawimage(pixels, &hetedansactie1, 192 - hetedansactie1.width, 64);
    } else {
        gfx_drawimage(pixels, &hetedansactie2, 192 - hetedansactie2.width, 64);
    }
}
