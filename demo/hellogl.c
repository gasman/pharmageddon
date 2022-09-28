#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>

#include "matrix.h"
#include "hellogl.h"

char debug_log[512];
const struct aiMesh *teapot;
unsigned int vertex_count;
unsigned int face_count;

GLfloat *teapot_vertices;
GLuint *teapot_faces;

static inline void mat4x4_ortho( mat4 out, float left, float right, float bottom, float top, float znear, float zfar )
{
    #define T(a, b) (a * 4 + b)

    out[T(0,0)] = 2.0f / (right - left);
    out[T(0,1)] = 0.0f;
    out[T(0,2)] = 0.0f;
    out[T(0,3)] = 0.0f;

    out[T(1,1)] = 2.0f / (top - bottom);
    out[T(1,0)] = 0.0f;
    out[T(1,2)] = 0.0f;
    out[T(1,3)] = 0.0f;

    out[T(2,2)] = -2.0f / (zfar - znear);
    out[T(2,0)] = 0.0f;
    out[T(2,1)] = 0.0f;
    out[T(2,3)] = 0.0f;

    out[T(3,0)] = -(right + left) / (right - left);
    out[T(3,1)] = -(top + bottom) / (top - bottom);
    out[T(3,2)] = -(zfar + znear) / (zfar - znear);
    out[T(3,3)] = 1.0f;

    #undef T
}

static const char * vertex_shader =
    "#version 330\n"
    "in vec3 i_position;\n"
    "in vec3 i_normal;\n"
    "out vec4 v_color;\n"
    "uniform mat4 u_transform;\n"
    "uniform mat4 u_project;\n"
    "void main() {\n"
    "    v_color = vec4(i_normal.x * 0.5 + 0.5, i_normal.y * 0.5 + 0.5, i_normal.z * 0.5 + 0.5, 1);\n"
    "    vec4 v_position = u_transform * vec4( i_position, 1.0 );\n"
    "    gl_Position = u_project * v_position;\n"
    "}\n";

static const char * fragment_shader =
    "#version 330\n"
    "in vec4 v_color;\n"
    "out vec4 o_color;\n"
    "void main() {\n"
    "    o_color = v_color;\n"
    "}\n";

GLuint vao, vbo, index_buffer;
GLint u_transform, u_project;
mat4 projection_matrix, transform_matrix;

void hellogl_init(void) {
    GLuint vs, fs, program;

    vs = glCreateShader( GL_VERTEX_SHADER );
    fs = glCreateShader( GL_FRAGMENT_SHADER );

    int length = strlen( vertex_shader );
    glShaderSource( vs, 1, ( const GLchar ** )&vertex_shader, &length );
    glCompileShader( vs );

    GLint status;
    glGetShaderiv( vs, GL_COMPILE_STATUS, &status );
    if( status == GL_FALSE )
    {
        glGetShaderInfoLog(vs, 512, NULL, debug_log);
        fprintf( stderr, "vertex shader compilation failed: %s\n", debug_log);
        return;
    }

    length = strlen( fragment_shader );
    glShaderSource( fs, 1, ( const GLchar ** )&fragment_shader, &length );
    glCompileShader( fs );

    glGetShaderiv( fs, GL_COMPILE_STATUS, &status );
    if( status == GL_FALSE )
    {
        glGetShaderInfoLog(fs, 512, NULL, debug_log);
        fprintf( stderr, "fragment shader compilation failed: %s\n", debug_log );
        return;
    }

    program = glCreateProgram();
    glAttachShader( program, vs );
    glAttachShader( program, fs );

    glBindAttribLocation( program, 0, "i_position" );
    glBindAttribLocation( program, 1, "i_normal" );

    glLinkProgram( program );

    glUseProgram( program );

    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glViewport( 0, 0, 192, 192 );

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

        teapot_vertices = malloc(vertex_count * 6 * sizeof(GLfloat));
        GLfloat *vpos = teapot_vertices;
        for (unsigned int i = 0; i < vertex_count; i++) {
            *vpos = (GLfloat)(teapot->mVertices[i].x);
            vpos++;
            *vpos = (GLfloat)(teapot->mVertices[i].y);
            vpos++;
            *vpos = (GLfloat)(teapot->mVertices[i].z);
            vpos++;
            *vpos = (GLfloat)(teapot->mNormals[i].x);
            vpos++;
            *vpos = (GLfloat)(teapot->mNormals[i].y);
            vpos++;
            *vpos = (GLfloat)(teapot->mNormals[i].z);
            vpos++;
        }

        teapot_faces = malloc(face_count * 3 * sizeof(GLuint));
        GLuint *fpos = teapot_faces;
        for (unsigned int i = 0; i < face_count; i++) {
            struct aiFace face = teapot->mFaces[i];
            *fpos = (GLuint)(face.mIndices[0]);
            fpos++;
            *fpos = (GLuint)(face.mIndices[1]);
            fpos++;
            *fpos = (GLuint)(face.mIndices[2]);
            fpos++;
        }
    } else {
        printf("obj import failed: %s\n", aiGetErrorString());
    }

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glEnableVertexAttribArray( 0 ); /* positions */
    glEnableVertexAttribArray( 1 ); /* normals */
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, 0 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, ( void * )(3 * sizeof(float)) );
    glBufferData( GL_ARRAY_BUFFER, vertex_count * 3 * sizeof(GLfloat), teapot_vertices, GL_STATIC_DRAW );

    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_count * 3 * sizeof(GLuint), teapot_faces, GL_STATIC_DRAW);

    mat4x4_ortho( projection_matrix, -5.0f, 5.0f, 5.0f, -5.0f, -30.0f, 100.0f );
    u_transform = glGetUniformLocation( program, "u_transform" );
    u_project = glGetUniformLocation( program, "u_project" );
}

void hellogl_frame(uint32_t *pixels, uint32_t time) {
    mat4_identity(transform_matrix);
    mat4_rotate_x(transform_matrix, ((float)time) / 2345.0);
    mat4_rotate_y(transform_matrix, ((float)time) / 1000.0);

    glUniformMatrix4fv( u_transform, 1, GL_FALSE, transform_matrix );
    glUniformMatrix4fv( u_project, 1, GL_FALSE, projection_matrix );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glBindVertexArray( vao );
    glDrawElements( GL_TRIANGLES, face_count * 3, GL_UNSIGNED_INT, 0 );

    glReadPixels(0, 0, 192, 192, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}
