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


const struct aiMesh *mesh;

double zbuffer[192*192];

gfx_image ambulance_texture;

typedef struct gfx3d_face {
    unsigned int index1;
    unsigned int index2;
    unsigned int index3;
} gfx3d_face;

typedef struct gfx3d_model {
    unsigned int vertex_count;
    vertex_in_attrs *vertices;
    vertex_out_attrs *transformed_vertices;

    unsigned int face_count;
    gfx3d_face *faces;
} gfx3d_model;

gfx3d_model ambulance;

void ambulance_init(void) {
    gfx_loadimage("../assets/patarty_raccoon_texture.png", &ambulance_texture);

    // Start the import on the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll t
    // probably to request more postprocessing than we do in this example.
    const struct aiScene* scene = aiImportFile( "../assets/patarty_raccoon.fbx",
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_GenSmoothNormals       |
        aiProcess_SortByPType);

    if (scene != NULL) {
        mesh = scene->mMeshes[0];
        ambulance.vertex_count = mesh->mNumVertices;
        ambulance.face_count = mesh->mNumFaces;

        ambulance.vertices = malloc(ambulance.vertex_count * sizeof(vertex_in_attrs));
        if (ambulance.vertices == NULL) {
            printf("could not allocate vertices\n");
        }

        struct aiVector3D *texture_coords = mesh->mTextureCoords[0];

        for (unsigned int i = 0; i < ambulance.vertex_count; i++) {
            struct aiVector3D v = mesh->mVertices[i];
            ambulance.vertices[i].position.x = v.x;
            ambulance.vertices[i].position.y = v.y;
            ambulance.vertices[i].position.z = v.z;
            struct aiVector3D n = mesh->mNormals[i];
            ambulance.vertices[i].normal.x = n.x;
            ambulance.vertices[i].normal.y = n.y;
            ambulance.vertices[i].normal.z = n.z;
            if (texture_coords != NULL) {
                struct aiVector3D t = texture_coords[i];
                ambulance.vertices[i].u = (int)(t.x * ambulance_texture.width);
                ambulance.vertices[i].v = (int)(t.y * ambulance_texture.height);
            }
        }

        ambulance.transformed_vertices = malloc(ambulance.vertex_count * sizeof(vertex_out_attrs));
        if (ambulance.transformed_vertices == NULL) {
            printf("could not allocate transformed_vertices\n");
        }

        ambulance.faces = malloc(ambulance.face_count * sizeof(gfx3d_face));
        if (ambulance.faces == NULL) {
            printf("could not allocate faces\n");
        }
        for (unsigned int i = 0; i < ambulance.face_count; i++) {
            struct aiFace face = mesh->mFaces[i];
            ambulance.faces[i].index1 = face.mIndices[0];
            ambulance.faces[i].index2 = face.mIndices[1];
            ambulance.faces[i].index3 = face.mIndices[2];
        }
    } else {
        printf("obj import failed: %s\n", aiGetErrorString());
    }
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
            pixels, zbuffer, &ambulance_texture, va0, va1, va2
        );
    }
}
