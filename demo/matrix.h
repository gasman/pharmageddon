typedef float mat4[16];
typedef float mat3[9];

void mat4_identity(mat4 out);
void mat4_mul(mat4 out, mat4 m1, mat4 m2);
void mat4_rotate_x(mat4 mat, float angle);
void mat4_rotate_y(mat4 mat, float angle);
void mat4_to_inverse_mat3(mat4 mat, mat3 dest);
