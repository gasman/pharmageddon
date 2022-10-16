#include "matrix.h"

#include <math.h>


void mat4_identity(mat4 out) {
    out[0] = 1;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    out[4] = 0;
    out[5] = 1;
    out[6] = 0;
    out[7] = 0;
    out[8] = 0;
    out[9] = 0;
    out[10] = 1;
    out[11] = 0;
    out[12] = 0;
    out[13] = 0;
    out[14] = 0;
    out[15] = 1;
}

void mat4_ortho(mat4 out, double left, double right, double bottom, double top, double znear, double zfar) {
    out[0] = 2.0 / (right - left);
    out[1] = 0.0;
    out[2] = 0.0;
    out[3] = 0.0;

    out[4] = 0.0;
    out[5] = 2.0 / (top - bottom);
    out[6] = 0.0;
    out[7] = 0.0;

    out[8] = 0.0;
    out[9] = 0.0;
    out[10] = -2.0 / (zfar - znear);
    out[11] = 0.0;

    out[12] = -(right + left) / (right - left);
    out[13] = -(top + bottom) / (top - bottom);
    out[14] = -(zfar + znear) / (zfar - znear);
    out[15] = 1.0;
}

void mat4_mul(mat4 out, mat4 m1, mat4 m2) {
    out[0] = m1[0]*m2[0] + m1[1]*m2[4] + m1[2]*m2[8] + m1[3]*m2[12];
    out[1] = m1[0]*m2[1] + m1[1]*m2[5] + m1[2]*m2[9] + m1[3]*m2[13];
    out[2] = m1[0]*m2[2] + m1[1]*m2[6] + m1[2]*m2[10] + m1[3]*m2[14];
    out[3] = m1[0]*m2[3] + m1[1]*m2[7] + m1[2]*m2[11] + m1[3]*m2[15];

    out[4] = m1[4]*m2[0] + m1[5]*m2[4] + m1[6]*m2[8] + m1[7]*m2[12];
    out[5] = m1[4]*m2[1] + m1[5]*m2[5] + m1[6]*m2[9] + m1[7]*m2[13];
    out[6] = m1[4]*m2[2] + m1[5]*m2[6] + m1[6]*m2[10] + m1[7]*m2[14];
    out[7] = m1[4]*m2[3] + m1[5]*m2[7] + m1[6]*m2[11] + m1[7]*m2[15];

    out[8] = m1[8]*m2[0] + m1[9]*m2[4] + m1[10]*m2[8] + m1[11]*m2[12];
    out[9] = m1[8]*m2[1] + m1[9]*m2[5] + m1[10]*m2[9] + m1[11]*m2[13];
    out[10] = m1[8]*m2[2] + m1[9]*m2[6] + m1[10]*m2[10] + m1[11]*m2[14];
    out[11] = m1[8]*m2[3] + m1[9]*m2[7] + m1[10]*m2[11] + m1[11]*m2[15];

    out[12] = m1[12]*m2[0] + m1[13]*m2[4] + m1[14]*m2[8] + m1[15]*m2[12];
    out[13] = m1[12]*m2[1] + m1[13]*m2[5] + m1[14]*m2[9] + m1[15]*m2[13];
    out[14] = m1[12]*m2[2] + m1[13]*m2[6] + m1[14]*m2[10] + m1[15]*m2[14];
    out[15] = m1[12]*m2[3] + m1[13]*m2[7] + m1[14]*m2[11] + m1[15]*m2[15];
}

vec3 mat4_mul_vec3(vec3 vec, mat4 mat) {
    double w = vec.x*mat[12] + vec.y*mat[13] + vec.z*mat[14] + mat[15];
    vec3 out = {
        (vec.x*mat[0] + vec.y*mat[1] + vec.z*mat[2] + mat[3])/w,
        (vec.x*mat[4] + vec.y*mat[5] + vec.z*mat[6] + mat[7])/w,
        (vec.x*mat[8] + vec.y*mat[9] + vec.z*mat[10] + mat[11])/w
    };
    return out;
}

vec3 mat3_mul_vec3(vec3 vec, mat3 mat) {
    vec3 out = {
        (vec.x*mat[0] + vec.y*mat[1] + vec.z*mat[2]),
        (vec.x*mat[3] + vec.y*mat[4] + vec.z*mat[5]),
        (vec.x*mat[6] + vec.y*mat[7] + vec.z*mat[8])
    };
    return out;
}

void mat4_rotate_x(mat4 mat, double angle) {
    double s = sin(angle);
    double c = cos(angle);
    double a10 = mat[4];
    double a11 = mat[5];
    double a12 = mat[6];
    double a13 = mat[7];
    double a20 = mat[8];
    double a21 = mat[9];
    double a22 = mat[10];
    double a23 = mat[11];

    mat[4] = a10 * c + a20 * s;
    mat[5] = a11 * c + a21 * s;
    mat[6] = a12 * c + a22 * s;
    mat[7] = a13 * c + a23 * s;

    mat[8] = a10 * -s + a20 * c;
    mat[9] = a11 * -s + a21 * c;
    mat[10] = a12 * -s + a22 * c;
    mat[11] = a13 * -s + a23 * c;
}

void mat4_rotate_y(mat4 mat, double angle) {
    double s = sin(angle);
    double c = cos(angle);
    double a00 = mat[0];
    double a01 = mat[1];
    double a02 = mat[2];
    double a03 = mat[3];
    double a20 = mat[8];
    double a21 = mat[9];
    double a22 = mat[10];
    double a23 = mat[11];

    mat[0] = a00 * c + a20 * -s;
    mat[1] = a01 * c + a21 * -s;
    mat[2] = a02 * c + a22 * -s;
    mat[3] = a03 * c + a23 * -s;

    mat[8] = a00 * s + a20 * c;
    mat[9] = a01 * s + a21 * c;
    mat[10] = a02 * s + a22 * c;
    mat[11] = a03 * s + a23 * c;
}

void mat4_translate(mat4 mat, vec3 vec) {
    mat[3] += vec.x;
    mat[7] += vec.y;
    mat[11] += vec.z;
}

void mat4_to_inverse_transpose_mat3(mat4 mat, mat3 dest) {
    /*
        Calculates the inverse of the upper 3x3 elements of a mat4 and copies the result into a mat3
        The resulting matrix is useful for calculating transformed normals
    */
    double a00 = mat[0];
    double a01 = mat[1];
    double a02 = mat[2];

    double a10 = mat[4];
    double a11 = mat[5];
    double a12 = mat[6];

    double a20 = mat[8];
    double a21 = mat[9];
    double a22 = mat[10];

    double b01 = a22 * a11 - a12 * a21;
    double b11 = -a22 * a10 + a12 * a20;
    double b21 = a21 * a10 - a11 * a20;

    double d = a00 * b01 + a01 * b11 + a02 * b21;

    double id = 1.0 / d;

    dest[0] = b01 * id;
    dest[3] = (-a22 * a01 + a02 * a21) * id;
    dest[6] = (a12 * a01 - a02 * a11) * id;
    dest[1] = b11 * id;
    dest[4] = (a22 * a00 - a02 * a20) * id;
    dest[7] = (-a12 * a00 + a02 * a10) * id;
    dest[2] = b21 * id;
    dest[5] = (-a21 * a00 + a01 * a20) * id;
    dest[8] = (a11 * a00 - a01 * a10) * id;
}

vec3 vec3_normalize(vec3 vec) {
    double len = sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
    vec3 result = {vec.x/len, vec.y/len, vec.z/len};
    return result;
}

double vec3_dot(vec3 v1, vec3 v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
