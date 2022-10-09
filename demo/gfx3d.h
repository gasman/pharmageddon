#include <stdint.h>

typedef struct vec3 {
    double x;
    double y;
    double z;
} vec3;

void gfx3d_flat_tri(uint32_t *pixels, double *zbuffer, vec3 v1, vec3 v2, vec3 v3, uint32_t colour);
