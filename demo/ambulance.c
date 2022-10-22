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

gfx3d_model stars;
#define STAR_COUNT 100
vertex_in_attrs star_vertices[STAR_COUNT];
vertex_out_attrs star_transformed_vertices[STAR_COUNT];


/* generate a random floating point number from min to max */
static double rand_range(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}


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

    stars.vertex_count = STAR_COUNT;
    stars.vertices = star_vertices;
    stars.transformed_vertices = star_transformed_vertices;
    stars.face_count = 0;
    for (int i = 0; i < STAR_COUNT; i++) {
        star_vertices[i].position.x = rand_range(0, 1);
        star_vertices[i].position.y = rand_range(0, 1);
        star_vertices[i].position.z = rand_range(0, 1);
    }
}

void ambulance_frame(uint32_t *pixels, uint32_t time) {
    mat4 camera_matrix;
    mat4 ambulance_matrix;
    mat3 normal_ambulance_matrix;

    gfx_cls(pixels, 0x00110000);
    gfx3d_clear_zbuffer(zbuffer);

    mat4 ambulance_matrix_final;
    mat4 stars_matrix, stars_matrix_final;
    vec3 light_pos;

    if (time < 3840) {
        mat4_identity(camera_matrix);
        mat4_rotate_y(camera_matrix, -1.3);
        vec3 trans_camera = {0, 0, 2 + ((double)time)*5/3840 };
        mat4_translate(camera_matrix, trans_camera);

        mat4_identity(ambulance_matrix);
        vec3 trans1 = {0, -2, 1-(double)time/1000};
        mat4_translate(ambulance_matrix, trans1);
        mat4_rotate_x(ambulance_matrix, sin(((double)time) / 400) / 8);

        mat4_mul(ambulance_matrix_final, camera_matrix, ambulance_matrix);
        mat4_to_inverse_transpose_mat3(ambulance_matrix_final, normal_ambulance_matrix);

        mat4_identity(stars_matrix);
        vec3 stars_trans = {-0.5, -0.5, -0.5};
        mat4_translate(stars_matrix, stars_trans);
        mat4_scale(stars_matrix, 16);
        mat4_mul(stars_matrix_final, camera_matrix, stars_matrix);

        light_pos.x = 0;
        light_pos.y = 10;
        light_pos.z = -2;
    } else {
        mat4_identity(camera_matrix);
        mat4_rotate_y(camera_matrix, 2.54-((double)time) / 1000);
        vec3 trans_camera = {0, 0, 7};
        mat4_translate(camera_matrix, trans_camera);

        mat4_identity(ambulance_matrix);
        vec3 trans1 = {0, -2, -3};
        mat4_translate(ambulance_matrix, trans1);
        mat4_rotate_x(ambulance_matrix, sin(((double)time) / 400) / 8);

        mat4_mul(ambulance_matrix_final, camera_matrix, ambulance_matrix);
        mat4_to_inverse_transpose_mat3(ambulance_matrix_final, normal_ambulance_matrix);

        mat4_identity(stars_matrix);
        vec3 stars_trans = {-0.5, -0.5, -0.5};
        mat4_translate(stars_matrix, stars_trans);
        mat4_scale(stars_matrix, 16);
        mat4_mul(stars_matrix_final, camera_matrix, stars_matrix);

        light_pos.x = 0;
        light_pos.y = 10;
        light_pos.z = -2;
    }

    gfx3d_gouraud_tex_mesh(pixels, zbuffer, ambulance, ambulance_matrix_final, normal_ambulance_matrix, light_pos);

    // tyres/steering wheel - dark grey
    gfx3d_gouraud_mesh(pixels, zbuffer, ambulance_bits[0], ambulance_matrix_final, normal_ambulance_matrix, light_pos, 0x40404000);

    // wheels/cab - mid grey
    gfx3d_gouraud_mesh(pixels, zbuffer, ambulance_bits[1], ambulance_matrix_final, normal_ambulance_matrix, light_pos, 0x80808000);

    // axles/window inner - light grey
    gfx3d_gouraud_mesh(pixels, zbuffer, ambulance_bits[2], ambulance_matrix_final, normal_ambulance_matrix, light_pos, 0xc0c0c000);

    // seats - green
    gfx3d_gouraud_mesh(pixels, zbuffer, ambulance_bits[3], ambulance_matrix_final, normal_ambulance_matrix, light_pos, 0x00800000);


    gfx3d_point_mesh_starfield(pixels, zbuffer, stars, stars_matrix_final, 0xffffff00, time);
}
