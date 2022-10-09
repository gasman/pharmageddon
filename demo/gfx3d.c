#include "gfx3d.h"
#include "gfx.h"

void gfx3d_flat_tri(uint32_t *pixels, double *zbuffer, vec3 v1, vec3 v2, vec3 v3, uint32_t colour) {
    gfx_filltri(
        pixels,
        (int)(96 + 96 * v1.x),
        (int)(96 - 96 * v1.y),
        (int)(96 + 96 * v2.x),
        (int)(96 - 96 * v2.y),
        (int)(96 + 96 * v3.x),
        (int)(96 - 96 * v3.y),
        colour
    );
}
