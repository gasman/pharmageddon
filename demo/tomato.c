#include <math.h>
#include <stdlib.h>
#include "gfx.h"
#include "tomato.h"

gfx_image tomato;

void tomato_init(void) {
    gfx_loadimage("../assets/tomato.png", &tomato);
}


void tomato_frame(uint32_t *pixels, uint32_t time) {
    gfx_cls(pixels, 0x00000000);
    double t = (double)time;

    gfx_drawimage(pixels, &tomato, 64, 80 - fabs(32*sin(t * M_PI/400)));
}
