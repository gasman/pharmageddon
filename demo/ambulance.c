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

    for (unsigned int i = 0; i < ambulance.vertex_count; i++) {
        vec3 pos = mat4_mul_vec3(ambulance.vertices[i].position, rotate_matrix);
        vec3 normal = mat3_mul_vec3(vec3_normalize(ambulance.vertices[i].normal), normal_rotate_matrix);
        // vec3 normal = ambulance.vertices[i].normal;
        vec3 light_dir = {light_pos.x - pos.x, light_pos.y - pos.y, light_pos.z - pos.z};
        light_dir = vec3_normalize(light_dir);
        double diffuse = vec3_dot(normal, light_dir);
        if (diffuse < 0) diffuse = 0;

        // hacky perspective transform
        vec3 projected_pos = {pos.x/pos.z, pos.y/pos.z, pos.z};
        ambulance.transformed_vertices[i].position = projected_pos;

        ambulance.transformed_vertices[i].brightness = diffuse;
        ambulance.transformed_vertices[i].u = ambulance.vertices[i].u;
        ambulance.transformed_vertices[i].v = ambulance.vertices[i].v;
    }

    for (double *zbuffer_ptr = zbuffer; zbuffer_ptr < (zbuffer + 192*192); zbuffer_ptr++) {
        *zbuffer_ptr = 100000;
    }

    for (unsigned int i = 0; i < ambulance.face_count; i++) {
        gfx3d_face face = ambulance.faces[i];
        vertex_out_attrs va0 = ambulance.transformed_vertices[face.index1];
        vertex_out_attrs va1 = ambulance.transformed_vertices[face.index2];
        vertex_out_attrs va2 = ambulance.transformed_vertices[face.index3];

        gfx3d_gouraud_tex_tri(
            pixels, zbuffer, &ambulance.texture, va0, va1, va2
        );
    }
}
