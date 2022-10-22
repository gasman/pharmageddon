#include <math.h>
#include <stdlib.h>
#include "gfx.h"
#include "farjan.h"

gfx_image farjan;
gfx_image waves;

void farjan_init(void) {
    gfx_loadimage("../assets/farjan.png", &farjan);
    gfx_loadimage("../assets/waves.png", &waves);
}


void farjan_frame(uint32_t *pixels, uint32_t time) {
    gfx_cls(pixels, 0x9999cc00);
    double t = (double)time;

    gfx_rotateimage(pixels, &farjan, 96, 96 + 8*cos((t-200)/400), -(double)time/20000, 0.3);
    gfx_drawimage(pixels, &waves, -32 + 8*sin(t/230), 100 + 8*cos(t/400));
    gfx_fillcircle(pixels, 128, 0, 24, 0xffff0000);
}
