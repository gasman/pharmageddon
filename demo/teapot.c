#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags
#include "gfx.h"
#include "gfx3d.h"
#include "teapot.h"

typedef struct vertex_in_attrs {
    vec3 position;
    vec3 normal;
} vertex_in_attrs;

typedef struct vertex_out_attrs {
    vec3 position;
    uint32_t colour;
} vertex_out_attrs;

const struct aiMesh *teapot;
unsigned int vertex_count;
unsigned int face_count;

double zbuffer[192*192];

vertex_in_attrs *model_vertices;
vertex_out_attrs *transformed_vertices;

void teapot_init(void) {
    // Start the import on the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll t
    // probably to request more postprocessing than we do in this example.
    const struct aiScene* scene = aiImportFile( "../assets/teapot.obj",
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_GenSmoothNormals       |
        aiProcess_SortByPType);

    if (scene != NULL) {
        teapot = scene->mMeshes[0];
        vertex_count = teapot->mNumVertices;
        face_count = teapot->mNumFaces;

        model_vertices = malloc(vertex_count * sizeof(vertex_in_attrs));
        if (model_vertices == NULL) {
            printf("could not allocate model_vertices\n");
        }

        for (unsigned int i = 0; i < vertex_count; i++) {
            struct aiVector3D v = teapot->mVertices[i];
            model_vertices[i].position.x = v.x;
            model_vertices[i].position.y = v.y;
            model_vertices[i].position.z = v.z;
            struct aiVector3D n = teapot->mNormals[i];
            model_vertices[i].normal.x = n.x;
            model_vertices[i].normal.y = n.y;
            model_vertices[i].normal.z = n.z;
        }

        transformed_vertices = malloc(vertex_count * sizeof(vertex_out_attrs));
        if (transformed_vertices == NULL) {
            printf("could not allocate transformed_vertices\n");
        }
    } else {
        printf("obj import failed: %s\n", aiGetErrorString());
    }
}


void teapot_frame(uint32_t *pixels, uint32_t time) {
    mat4 rotate_matrix;
    mat3 normal_rotate_matrix;
    // mat4 projection_matrix;

    gfx_cls(pixels, 0x00110000);

    mat4_identity(rotate_matrix);
    vec3 trans1 = {0, -1, 1};
    mat4_translate(rotate_matrix, trans1);
    mat4_rotate_y(rotate_matrix, ((double)time) / 1000);
    mat4_rotate_x(rotate_matrix, ((double)time) / 900);
    // mat4_ortho( projection_matrix, -5.0, 5.0, 5.0, -5.0, -30.0, 100.0 );
    mat4_to_inverse_mat3(rotate_matrix, normal_rotate_matrix);

    vec3 light_pos = {0, 5, 0};

    for (unsigned int i = 0; i < vertex_count; i++) {
        vec3 pos = mat4_mul_vec3(model_vertices[i].position, rotate_matrix);
        vec3 normal = mat3_mul_vec3(model_vertices[i].normal, normal_rotate_matrix);
        vec3 light_dir = {light_pos.x - pos.x, light_pos.y - pos.y, light_pos.z - pos.z};
        light_dir = vec3_normalize(light_dir);
        double diffuse = vec3_dot(normal, light_dir);
        if (diffuse < 0) diffuse = 0;

        transformed_vertices[i].position = pos; // mat4_mul_vec3(pos, projection_matrix);
        transformed_vertices[i].colour = ((uint32_t)(diffuse * 200)) * 0x01010100;
    }

    for (double *zbuffer_ptr = zbuffer; zbuffer_ptr < (zbuffer + 192*192); zbuffer_ptr++) {
        *zbuffer_ptr = 100000;
    }

    for (unsigned int i = 0; i < face_count; i++) {
        struct aiFace face = teapot->mFaces[i];
        vertex_out_attrs va0 = transformed_vertices[face.mIndices[0]];
        vertex_out_attrs va1 = transformed_vertices[face.mIndices[1]];
        vertex_out_attrs va2 = transformed_vertices[face.mIndices[2]];

        vec3 v0 = va0.position;
        vec3 v1 = va1.position;
        vec3 v2 = va2.position;

        vec3 sv1 = {v0.x/5, v0.y/5, v0.z};
        vec3 sv2 = {v1.x/5, v1.y/5, v1.z};
        vec3 sv3 = {v2.x/5, v2.y/5, v2.z};
        gfx3d_flat_tri(
            pixels, zbuffer, sv1, sv2, sv3,
            va0.colour
        );
    }
}
