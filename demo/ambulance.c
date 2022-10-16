#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"
#include "gfx3d.h"
#include "ambulance.h"

double zbuffer[192*192];

gfx3d_model ambulance;

void ambulance_init(void) {
    gfx3d_load_model("../assets/patarty_raccoon.fbx", "../assets/patarty_raccoon_texture.png", &ambulance);
}

void ambulance_frame(uint32_t *pixels, uint32_t time) {
    mat4 rotate_matrix;
    mat3 normal_rotate_matrix;
    // mat4 projection_matrix;

    gfx_cls(pixels, 0x00110000);
    gfx3d_clear_zbuffer(zbuffer);

    mat4_identity(rotate_matrix);
    vec3 trans1 = {0, -2, 1};
    mat4_translate(rotate_matrix, trans1);
    mat4_rotate_y(rotate_matrix, ((double)time) / 1000);
    // mat4_rotate_x(rotate_matrix, ((double)time) / 900);
    // mat4_ortho( projection_matrix, -5.0, 5.0, 5.0, -5.0, -30.0, 100.0 );
    vec3 trans2 = {0, 0, 7};
    mat4_translate(rotate_matrix, trans2);
    mat4_to_inverse_transpose_mat3(rotate_matrix, normal_rotate_matrix);

    vec3 light_pos = {0, 10, -2};

    gfx3d_gouraud_tex_mesh(pixels, zbuffer, ambulance, rotate_matrix, normal_rotate_matrix, light_pos);
}
