#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags
#include "gfx.h"
#include "gfx3d.h"
#include "ambulance.h"

double zbuffer[192*192];

gfx3d_model ambulance;
gfx3d_model ambulance_bits[4];

void ambulance_init(void) {
    gfx_loadimage("../assets/ambulance_mat_2D_View.png", &ambulance.texture);

    // Start the import on the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll t
    // probably to request more postprocessing than we do in this example.
    const struct aiScene* scene = aiImportFile("../assets/ambulance.fbx",
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_GenSmoothNormals       |
        aiProcess_FlipWindingOrder |
        aiProcess_SortByPType);

    if (scene != NULL) {
        gfx3d_read_mesh(scene, 0, &ambulance);
        gfx3d_read_mesh(scene, 1, &ambulance_bits[0]);
        gfx3d_read_mesh(scene, 2, &ambulance_bits[1]);
        gfx3d_read_mesh(scene, 3, &ambulance_bits[2]);
        gfx3d_read_mesh(scene, 4, &ambulance_bits[3]);
    } else {
        printf("obj import failed: %s\n", aiGetErrorString());
    }
}

void ambulance_frame(uint32_t *pixels, uint32_t time) {
    mat4 rotate_matrix;
    mat3 normal_rotate_matrix;
    // mat4 projection_matrix;

    gfx_cls(pixels, 0x00110000);
    gfx3d_clear_zbuffer(zbuffer);

    mat4_identity(rotate_matrix);
    vec3 trans1 = {0, -2, -3};
    mat4_translate(rotate_matrix, trans1);
    mat4_rotate_y(rotate_matrix, ((double)time) / 1000);
    // mat4_rotate_x(rotate_matrix, ((double)time) / 900);
    // mat4_ortho( projection_matrix, -5.0, 5.0, 5.0, -5.0, -30.0, 100.0 );
    vec3 trans2 = {0, 0, 7};
    mat4_translate(rotate_matrix, trans2);
    mat4_to_inverse_transpose_mat3(rotate_matrix, normal_rotate_matrix);

    vec3 light_pos = {0, 10, -2};

    gfx3d_gouraud_tex_mesh(pixels, zbuffer, ambulance, rotate_matrix, normal_rotate_matrix, light_pos);

    // tyres/steering wheel - dark grey
    gfx3d_gouraud_mesh(pixels, zbuffer, ambulance_bits[0], rotate_matrix, normal_rotate_matrix, light_pos, 0x40404000);

    // wheels/cab - mid grey
    gfx3d_gouraud_mesh(pixels, zbuffer, ambulance_bits[1], rotate_matrix, normal_rotate_matrix, light_pos, 0x80808000);

    // axles/window inner - light grey
    gfx3d_gouraud_mesh(pixels, zbuffer, ambulance_bits[2], rotate_matrix, normal_rotate_matrix, light_pos, 0xc0c0c000);

    // seats - green
    gfx3d_gouraud_mesh(pixels, zbuffer, ambulance_bits[3], rotate_matrix, normal_rotate_matrix, light_pos, 0x00800000);
}
