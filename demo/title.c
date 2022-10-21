#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags
#include "gfx.h"
#include "gfx3d.h"
#include "title.h"

double zbuffer[192*192];

gfx3d_model virus0;
gfx3d_model virus1;
gfx_image title_image;
gfx_image background;

void title_init(void) {
    gfx_loadimage("../assets/pharmageddon.png", &title_image);
    gfx_loadimage("../assets/lavalamp-green2.png", &background);

    // Start the import on the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll t
    // probably to request more postprocessing than we do in this example.
    const struct aiScene* scene = aiImportFile("../assets/virus.fbx",
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_GenSmoothNormals       |
        aiProcess_FlipWindingOrder |
        aiProcess_SortByPType);

    if (scene != NULL) {
        gfx3d_read_mesh(scene, 0, &virus0);
        gfx3d_read_mesh(scene, 1, &virus1);
    } else {
        printf("obj import failed: %s\n", aiGetErrorString());
    }
}

void title_frame(uint32_t *pixels, uint32_t time) {
    mat4 rotate_matrix;
    mat3 normal_rotate_matrix;
    // mat4 projection_matrix;

    gfx_cls(pixels, 0x00110000);
    gfx3d_clear_zbuffer(zbuffer);

    mat4_identity(rotate_matrix);
    mat4_rotate_y(rotate_matrix, 1);
    mat4_rotate_x(rotate_matrix, ((double)time) / 2000);
    // mat4_ortho( projection_matrix, -5.0, 5.0, 5.0, -5.0, -30.0, 100.0 );
    vec3 trans2 = {0, ((double)time) / 4000 - 4, 2.5};
    mat4_translate(rotate_matrix, trans2);
    mat4_to_inverse_transpose_mat3(rotate_matrix, normal_rotate_matrix);

    vec3 light_pos = {5 - ((double)time) / 4000, 1, 0};

    gfx_rotateimage(pixels, &background, 96, 96, (double)time/20000, 0.6);

    gfx3d_gouraud_mesh(pixels, zbuffer, virus0, rotate_matrix, normal_rotate_matrix, light_pos, 0x00ff0000);
    gfx3d_gouraud_mesh(pixels, zbuffer, virus1, rotate_matrix, normal_rotate_matrix, light_pos, 0x60a06000);

    gfx_drawimage(pixels, &title_image, 0, 64);
}
