#include <stdlib.h>
#include "gfx.h"
#include "nyancat.h"

gfx_image nyancat;

void nyancat_init(void) {
    gfx_loadimage("../assets/nyancat.png", &nyancat);
}

void nyancat_frame(uint32_t *pixels, uint32_t time) {
    int frm = (time / 70) % 12;
    gfx_drawimage(pixels, &nyancat, 0, frm * -192);
}
