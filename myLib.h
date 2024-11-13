#ifndef _MYLIB_H_

#define _MYLIB_H_
typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec4;

typedef struct {
    float x;
    float y;
    float z;
} vec3;

typedef struct {
    float x;
    float y;
} vec2;

typedef struct {
    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
} mat4;

typedef struct {
    vec3 x;
    vec3 y;
    vec3 z;
    
}mat3;
#define DEG_TO_RAD(angle_in_degrees) (angle_in_degrees * M_PI / 180.0)

// Insert function signatures after this line

void print_v4(vec4);
vec4 scalar_v4(vec4,float);
vec4 add_v4(vec4,vec4);
vec4 sub_v4(vec4,vec4);
float magnitude_v4(vec4);
vec4 normalize_v4(vec4);
float dot_v4(vec4,vec4);
vec4 cross_product_v4(vec4,vec4);
void print_m4(mat4);
mat4 scalar_m4(mat4,float);
mat4 add_m4(mat4,mat4);
mat4 sub_m4(mat4,mat4);
mat4 transpose_m4(mat4);
vec4 multiply_m4_vec4(mat4,vec4);
mat4 multiply_m4_m4(mat4,mat4);
mat4 inverse_m4(mat4);
mat4 minor_m4(mat4);
mat4 cofactor_m4(mat4);
float determinant_m4(mat4);
mat4 translation_m4(float,float,float);
mat4 scaling_m4(float,float,float);
mat4 rotating_x_m4(float);
mat4 rotating_y_m4(float);
mat4 rotating_z_m4(float);
mat4 m4_identity();
mat4 rotation_axis_angle_m4(vec4, float);

// Do not put anything after this line

#endif
