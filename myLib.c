#include "myLib.h"
#include <stdio.h>
#include <math.h>


void print_v4(vec4 v) {
    printf("[ %7.3f %7.3f %7.3f %7.3f ]\n", v.x, v.y, v.z, v.w);
}

vec4 scalar_v4(vec4 v, float s){
    v.x *= s;
    v.y *= s;
    v.w *= s;
    v.z *= s;
    return v;
}

vec4 add_v4(vec4 v1, vec4 v2){
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    v1.w += v2.w;
    return v1;
}

vec4 sub_v4(vec4 v1, vec4 v2){
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    v1.w -= v2.w;
    return v1;
}

float magnitude_v4(vec4 v){
    //sqrt x^2 + y^2 ...
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

vec4 normalize_v4(vec4 v){
    return scalar_v4(v,1.0 / magnitude_v4(v));
}

float dot_v4(vec4 v1, vec4 v2){
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

vec4 cross_product_v4(vec4 v1, vec4 v2){
    // last value always 0
    vec4 vec = {0,0,0,0};
    vec.x = (v1.y * v2.z) - (v1.z * v2.y);
    vec.y = (v1.z * v2.x) - (v1.x * v2.z);
    vec.z = (v1.x * v2.y) - (v1.y * v2.x);
    return vec;
}

void print_m4(mat4 m){
    //print each elemnet seperatly is necessary
    printf("\n");
    printf("[ %7.3f %7.3f %7.3f %7.3f ]\n", m.x.x, m.y.x, m.z.x, m.w.x);
    printf("[ %7.3f %7.3f %7.3f %7.3f ]\n", m.x.y, m.y.y, m.z.y, m.w.y);
    printf("[ %7.3f %7.3f %7.3f %7.3f ]\n", m.x.z, m.y.z, m.z.z, m.w.z);
    printf("[ %7.3f %7.3f %7.3f %7.3f ]\n", m.x.w, m.y.w, m.z.w, m.w.w);


    printf("\n");
}

mat4 scalar_m4(mat4 m1, float s){
    //just multiply everything by the s
    m1.x = scalar_v4(m1.x,s);
    m1.y = scalar_v4(m1.y,s);
    m1.z = scalar_v4(m1.z,s);
    m1.w = scalar_v4(m1.w,s);
    return m1;
}

mat4 add_m4(mat4 m1, mat4 m2){
    //just add all vecs
    m1.x = add_v4(m1.x,m2.x);
    m1.y = add_v4(m1.y,m2.y);
    m1.z = add_v4(m1.z,m2.z);
    m1.w = add_v4(m1.w,m2.w);
    return m1;
}

mat4 sub_m4(mat4 m1, mat4 m2){
    //just sub all vecs
    m1.x = sub_v4(m1.x,m2.x);
    m1.y = sub_v4(m1.y,m2.y);
    m1.z = sub_v4(m1.z,m2.z);
    m1.w = sub_v4(m1.w,m2.w);
    return m1;
}

mat4 transpose_m4(mat4 m){
    mat4 n;
    //making each row into a vecotor
    vec4 x = {m.x.x,m.y.x,m.z.x,m.w.x};
    vec4 y = {m.x.y,m.y.y,m.z.y,m.w.y};
    vec4 z = {m.x.z,m.y.z,m.z.z,m.w.z};
    vec4 w = {m.x.w,m.y.w,m.z.w,m.w.w};
    //adding all the vectors into the new matrix
    n.x = x;
    n.y = y;
    n.z = z;
    n.w = w;
    return n;
}


 vec4 multiply_m4_vec4(mat4 m, vec4 v){
    //each entry is the sum of the vector in the matrix times the entry
    vec4 a;
    a.x = m.x.x * v.x + m.y.x * v.y + m.z.x * v.z + m.w.x * v.w;
    a.y = m.x.y * v.x + m.y.y * v.y + m.z.y * v.z + m.w.y * v.w;
    a.z = m.x.z * v.x + m.y.z * v.y + m.z.z * v.z + m.w.z * v.w;
    a.w = m.x.w * v.x + m.y.w * v.y + m.z.w * v.z + m.w.w * v.w;
    return a;
}

    //multiply row from column 1 with column from row two
// Structure definitions for mat4 and vec4
// Function to multiply two column-major 4x4 matrices
mat4 multiply_m4_m4(mat4 m1, mat4 m2) {
    mat4 a;

    a.x.x = (m1.x.x * m2.x.x) + (m1.y.x * m2.x.y) + (m1.z.x * m2.x.z) + (m1.w.x * m2.x.w);
    a.x.y = (m1.x.y * m2.x.x) + (m1.y.y * m2.x.y) + (m1.z.y * m2.x.z) + (m1.w.y * m2.x.w);
    a.x.z = (m1.x.z * m2.x.x) + (m1.y.z * m2.x.y) + (m1.z.z * m2.x.z) + (m1.w.z * m2.x.w);
    a.x.w = (m1.x.w * m2.x.x) + (m1.y.w * m2.x.y) + (m1.z.w * m2.x.z) + (m1.w.w * m2.x.w);

    a.y.x = (m1.x.x * m2.y.x) + (m1.y.x * m2.y.y) + (m1.z.x * m2.y.z) + (m1.w.x * m2.y.w);
    a.y.y = (m1.x.y * m2.y.x) + (m1.y.y * m2.y.y) + (m1.z.y * m2.y.z) + (m1.w.y * m2.y.w);
    a.y.z = (m1.x.z * m2.y.x) + (m1.y.z * m2.y.y) + (m1.z.z * m2.y.z) + (m1.w.z * m2.y.w);
    a.y.w = (m1.x.w * m2.y.x) + (m1.y.w * m2.y.y) + (m1.z.w * m2.y.z) + (m1.w.w * m2.y.w);

    a.z.x = (m1.x.x * m2.z.x) + (m1.y.x * m2.z.y) + (m1.z.x * m2.z.z) + (m1.w.x * m2.z.w);
    a.z.y = (m1.x.y * m2.z.x) + (m1.y.y * m2.z.y) + (m1.z.y * m2.z.z) + (m1.w.y * m2.z.w);
    a.z.z = (m1.x.z * m2.z.x) + (m1.y.z * m2.z.y) + (m1.z.z * m2.z.z) + (m1.w.z * m2.z.w);
    a.z.w = (m1.x.w * m2.z.x) + (m1.y.w * m2.z.y) + (m1.z.w * m2.z.z) + (m1.w.w * m2.z.w);

    a.w.x = (m1.x.x * m2.w.x) + (m1.y.x * m2.w.y) + (m1.z.x * m2.w.z) + (m1.w.x * m2.w.w);
    a.w.y = (m1.x.y * m2.w.x) + (m1.y.y * m2.w.y) + (m1.z.y * m2.w.z) + (m1.w.y * m2.w.w);
    a.w.z = (m1.x.z * m2.w.x) + (m1.y.z * m2.w.y) + (m1.z.z * m2.w.z) + (m1.w.z * m2.w.w);
    a.w.w = (m1.x.w * m2.w.x) + (m1.y.w * m2.w.y) + (m1.z.w * m2.w.z) + (m1.w.w * m2.w.w);

    return a;
}

mat4 minor_m4(mat4 m){
    mat4 n = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    n.x.x = m.y.y* m.z.z * m.w.w +
     m.z.y * m.w.z * m.y.w +
     m.w.y * m.y.z * m.z.w -
     m.y.w * m.z.z * m.w.y - 
     m.z.w * m.w.z * m.y.y -
     m.w.w * m.y.z * m.z.y;
     
     n.y.x = m.x.y* m.z.z * m.w.w +
     m.z.y * m.w.z * m.x.w +
     m.w.y * m.x.z * m.z.w -
     m.x.w * m.z.z * m.w.y - 
     m.z.w * m.w.z * m.x.y -
     m.w.w * m.x.z * m.z.y;

     n.z.x = m.x.y* m.y.z * m.w.w +
     m.y.y * m.w.z * m.x.w +
     m.w.y * m.x.z * m.y.w -
     m.x.w * m.y.z * m.w.y - 
     m.y.w * m.w.z * m.x.y -
     m.w.w * m.x.z * m.y.y;

     n.w.x = m.x.y* m.y.z * m.z.w +
     m.y.y * m.z.z * m.x.w +
     m.z.y * m.x.z * m.y.w -
     m.x.w * m.y.z * m.z.y - 
     m.y.w * m.z.z * m.x.y -
     m.z.w * m.x.z * m.y.y;

     //row2
     n.x.y = m.y.x* m.z.z * m.w.w +
     m.z.x * m.w.z * m.y.w +
     m.w.x * m.y.z * m.z.w -
     m.y.w * m.z.z * m.w.x - 
     m.z.w * m.w.z * m.y.x -
     m.w.w * m.y.z * m.z.x;
     
     n.y.y = m.x.x* m.z.z * m.w.w +
     m.z.x * m.w.z * m.x.w +
     m.w.x * m.x.z * m.z.w -
     m.x.w * m.z.z * m.w.x - 
     m.z.w * m.w.z * m.x.x -
     m.w.w * m.x.z * m.z.x;

     n.z.y= m.x.x* m.y.z * m.w.w +
     m.y.x * m.w.z * m.x.w +
     m.w.x * m.x.z * m.y.w -
     m.x.w * m.y.z * m.w.x - 
     m.y.w * m.w.z * m.x.x -
     m.w.w * m.x.z * m.y.x;

     n.w.y = m.x.x* m.y.z * m.z.w +
     m.y.x * m.z.z * m.x.w +
     m.z.x * m.x.z * m.y.w -
     m.x.w * m.y.z * m.z.x - 
     m.y.w * m.z.z * m.x.x -
     m.z.w * m.x.z * m.y.x;
    
    //row3
     n.x.z = m.y.x* m.z.y * m.w.w +
     m.z.x * m.w.y * m.y.w +
     m.w.x * m.y.y * m.z.w -
     m.y.w * m.z.y * m.w.x - 
     m.z.w * m.w.y * m.y.x -
     m.w.w * m.y.y * m.z.x;
     
     n.y.z = m.x.x* m.z.y * m.w.w +
     m.z.x * m.w.y * m.x.w +
     m.w.x * m.x.y * m.z.w -
     m.x.w * m.z.y * m.w.x - 
     m.z.w * m.w.y * m.x.x -
     m.w.w * m.x.y * m.z.x;

     n.z.z= m.x.x* m.y.y * m.w.w +
     m.y.x * m.w.y * m.x.w +
     m.w.x * m.x.y * m.y.w -
     m.x.w * m.y.y * m.w.x - 
     m.y.w * m.w.y * m.x.x -
     m.w.w * m.x.y * m.y.x;

     n.w.z = m.x.x* m.y.y * m.z.w +
     m.y.x * m.z.y * m.x.w +
     m.z.x * m.x.y * m.y.w -
     m.x.w * m.y.y * m.z.x - 
     m.y.w * m.z.y * m.x.x -
     m.z.w * m.x.y * m.y.x;

    //row4
     n.x.w = m.y.x* m.z.y * m.w.z +
     m.z.x * m.w.y * m.y.z +
     m.w.x * m.y.y * m.z.z -
     m.y.z * m.z.y * m.w.x - 
     m.z.z * m.w.y * m.y.x -
     m.w.z * m.y.y * m.z.x;
     
     n.y.w = m.x.x* m.z.y * m.w.z +
     m.z.x * m.w.y * m.x.z +
     m.w.x * m.x.y * m.z.z -
     m.x.z * m.z.y * m.w.x - 
     m.z.z * m.w.y * m.x.x -
     m.w.z * m.x.y * m.z.x;

     n.z.w= m.x.x* m.y.y * m.w.z +
     m.y.x * m.w.y * m.x.z +
     m.w.x * m.x.y * m.y.z -
     m.x.z * m.y.y * m.w.x - 
     m.y.z * m.w.y * m.x.x -
     m.w.z * m.x.y * m.y.x;

     n.w.w = m.x.x* m.y.y * m.z.z +
     m.y.x * m.z.y * m.x.z +
     m.z.x * m.x.y * m.y.z -
     m.x.z * m.y.y * m.z.x - 
     m.y.z * m.z.y * m.x.x -
     m.z.z * m.x.y * m.y.x;
    return n;
}
mat4 cofactor_m4(mat4 m){
    m.x.y = -m.x.y;
    m.x.w = -m.x.w;
    m.y.x = -m.y.x;
    m.y.z = -m.y.z;
    m.z.y = -m.z.y;
    m.z.w = -m.z.w;
    m.w.x = -m.w.x;
    m.w.z = -m.w.z;
    return m;
}
mat4 inverse_m4(mat4 m){
    mat4 a = minor_m4(m);
    float det = a.x.x * m.x.x - a.x.y * m.x.y + a.x.z * m.x.z - a.x.w * m.x.w;
    return scalar_m4(transpose_m4(cofactor_m4(a)),1 / det);
}

mat4 translation_m4(float a1, float a2, float a3){
    mat4 a = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{a1,a2,a3,1}};
    return a;
}

mat4 scaling_m4(float a1, float a2, float a3){
    mat4 a = {{a1,0,0,0},{0,a2,0,0},{0,0,a3,0},{0,0,0,1}};
    return a;
}
 
mat4 rotating_x_m4(float angle){
    float n = DEG_TO_RAD(angle);
    mat4 a = {{1,0,0,0},{0,cos(n),sin(n),0},{0,-sin(n),cos(n),0},{0,0,0,1}};
    return a;
}
mat4 rotating_y_m4(float angle){
    float n = DEG_TO_RAD(angle);
    mat4 a = {{cos(n),0,-sin(n),0},{0,1,0,0},{sin(n),0,cos(n),0},{0,0,0,1}};
    return a;
}

mat4 rotating_z_m4(float angle){
    float n = DEG_TO_RAD(angle);
    mat4 a = {{cos(n),sin(n),0,0},{-sin(n),cos(n),0,0},{0,0,1,0},{0,0,0,1}};
    return a;
}

//https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
//rotation matrix from axis and angle
mat4 rotation_axis_angle_m4(vec4 axis, float angle) {
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    float c = cos(angle);  
    float s = sin(angle);      

    mat4 a;

    a.x.x = (1 - c) * x * x + c;
    a.x.y = (1 - c) * x * y - s * z;
    a.x.z = (1 - c) * x * z + s * y;
    a.x.w = 0;

    a.y.x = (1 - c) * x * y + s * z;
    a.y.y = (1 - c) * y * y + c;
    a.y.z = (1 - c) * y * z - s * x;
    a.y.w = 0;

    a.z.x = (1 - c) * x * z - s * y;
    a.z.y = (1 - c) * y * z + s * x;
    a.z.z = (1 - c) * z * z + c;
    a.z.w = 0;

    a.w.x = 0;
    a.w.y = 0;
    a.w.z = 0;
    a.w.w = 1;

    return a;
}



// mat4 ortho(float left, float right, float bottom, float top, float near, float far) {
//     mat4 ortho_matrix = {
//         {2 / (right - left), 0, 0, 0},
//         {0, 2 / (top - bottom), 0, 0},
//         {0, 0, -2 / (far - near), 0},
//         {-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1}
//     };
//     return ortho_matrix;
// }

mat4 look_at(vec4 eye, vec4 center, vec4 up) {
    vec4 forward = normalize_v4(sub_v4(center, eye));
    vec4 right = normalize_v4(cross_product_v4(forward, up));
    vec4 true_up = cross_product_v4(right, forward);
    mat4 view_matrix = {{right.x, true_up.x, -forward.x, 0},{right.y, true_up.y, -forward.y, 0},{right.z, true_up.z, -forward.z, 0},{ -dot_v4(right, eye),-dot_v4(true_up, eye),dot_v4(forward, eye),1}};
    return view_matrix;
}

mat4 frustum(float left, float right, float bottom, float top, float near, float far) {
    mat4 frustum_matrix = {
        {2 * near / (right - left), 0, 0, 0},
        {0, 2 * near / (top - bottom), 0, 0},
        {   (right + left) / (right - left),
            (top + bottom) / (top - bottom),
            -(far + near) / (far - near),
            -1},{0, 0, -2 * far * near / (far - near), 0}};
    return frustum_matrix;
}
