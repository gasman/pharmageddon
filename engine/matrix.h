typedef struct vec3 {
    double x;
    double y;
    double z;
} vec3;

typedef double mat4[16];
typedef double mat3[9];

void mat4_identity(mat4 out);
void mat4_ortho(mat4 out, double left, double right, double bottom, double top, double znear, double zfar);
void mat4_mul(mat4 out, mat4 m1, mat4 m2);
vec3 mat4_mul_vec3(vec3 vec, mat4 mat);
vec3 mat3_mul_vec3(vec3 vec, mat3 mat);
void mat4_rotate_x(mat4 mat, double angle);
void mat4_rotate_y(mat4 mat, double angle);
void mat4_translate(mat4 mat, vec3 vec);
void mat4_to_inverse_transpose_mat3(mat4 mat, mat3 dest);

vec3 vec3_normalize(vec3 vec);
double vec3_dot(vec3 v1, vec3 v2);
