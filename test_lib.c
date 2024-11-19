#include "myLib.h"
#include <stdio.h>
#include <math.h>

// Helper function to test equality of two vec4s
int compare_vec4(vec4 v1, vec4 v2) {
    return (fabs(v1.x - v2.x) < 1e-6 &&
            fabs(v1.y - v2.y) < 1e-6 &&
            fabs(v1.z - v2.z) < 1e-6 &&
            fabs(v1.w - v2.w) < 1e-6);
}

// Helper function to test equality of two mat4s
int compare_mat4(mat4 m1, mat4 m2) {
    return compare_vec4(m1.x, m2.x) &&
           compare_vec4(m1.y, m2.y) &&
           compare_vec4(m1.z, m2.z) &&
           compare_vec4(m1.w, m2.w);
}

// Test functions
void test_vector_operations() {
    vec4 v1 = {1.0, 2.0, 3.0, 4.0};
    vec4 v2 = {4.0, 3.0, 2.0, 1.0};

    printf("Testing Vector Operations...\n");
    vec4 result;

    // Test scalar multiplication
    result = scalar_v4(v1, 2.0);
    print_v4(result);

    // Test addition
    result = add_v4(v1, v2);
    print_v4(result);

    // Test subtraction
    result = sub_v4(v1, v2);
    print_v4(result);

    // Test magnitude
    printf("Magnitude of v1: %.6f\n", magnitude_v4(v1));

    // Test normalization
    result = normalize_v4(v1);
    print_v4(result);

    // Test dot product
    printf("Dot product of v1 and v2: %.6f\n", dot_v4(v1, v2));

    // Test cross product
    vec4 v3 = {1.0, 0.0, 0.0, 0.0};
    vec4 v4 = {0.0, 1.0, 0.0, 0.0};
    result = cross_product_v4(v3, v4);
    print_v4(result);
}

void test_matrix_operations() {
    mat4 m1 = {{1, 2, 3, 4},
               {5, 6, 7, 8},
               {9, 10, 11, 12},
               {13, 14, 15, 16}};
    mat4 m2 = {{16, 15, 14, 13},
               {12, 11, 10, 9},
               {8, 7, 6, 5},
               {4, 3, 2, 1}};

    vec4 v = {1.0, 2.0, 3.0, 1.0};

    printf("Testing Matrix Operations...\n");

    // Test scalar multiplication
    mat4 result_m = scalar_m4(m1, 2.0);
    print_m4(result_m);

    // Test addition
    result_m = add_m4(m1, m2);
    print_m4(result_m);

    // Test subtraction
    result_m = sub_m4(m1, m2);
    print_m4(result_m);

    // Test matrix transpose
    result_m = transpose_m4(m1);
    print_m4(result_m);

    // Test matrix-vector multiplication
    vec4 result_v = multiply_m4_vec4(m1, v);
    print_v4(result_v);

    // Test matrix-matrix multiplication
    result_m = multiply_m4_m4(m1, m2);
    print_m4(result_m);
}

void test_advanced_matrix_operations() {
    mat4 m = {{3, 0, 2, 0},
              {2, 0, -2, 0},
              {0, 1, 1, 0},
              {0, 0, 0, 1}};

    printf("Testing Advanced Matrix Operations...\n");

    // Test minor and cofactor
    mat4 minor_result = minor_m4(m);
    printf("Minor Matrix:\n");
    print_m4(minor_result);

    mat4 cofactor_result = cofactor_m4(minor_result);
    printf("Cofactor Matrix:\n");
    print_m4(cofactor_result);

    // Test inverse
    mat4 inverse_result = inverse_m4(m);
    printf("Inverse Matrix:\n");
    print_m4(inverse_result);

    // Test transformations
    mat4 translation = translation_m4(1.0, 2.0, 3.0);
    printf("Translation Matrix:\n");
    print_m4(translation);

    mat4 scaling = scaling_m4(2.0, 2.0, 2.0);
    printf("Scaling Matrix:\n");
    print_m4(scaling);

    mat4 rotation = rotating_y_m4(45.0);
    printf("Rotation Matrix (Y-axis):\n");
    print_m4(rotation);
}

void test_frustum_and_look_at() {
    printf("Testing Frustum and Look-At Matrices...\n");

    // Test frustum
    mat4 frustum_matrix = frustum(-1, 1, -1, 1, 1, 10);
    printf("Frustum Matrix:\n");
    print_m4(frustum_matrix);

    // Test look-at
    vec4 eye = {0.0, 0.0, 0, 0};
    vec4 center = {0.0, 0.0, -1, 0};
    vec4 up = {0.0, 1.0, 0.0, 0.0};
    mat4 look_at_matrix = look_at(eye, center, up);
    printf("Look-At Matrix:\n");
    print_m4(look_at_matrix);
}

int main() {
    printf("Running Tests for myLib.h\n");

    // Test vector operations
    test_vector_operations();

    // Test matrix operations
    test_matrix_operations();

    // Test advanced matrix operations
    test_advanced_matrix_operations();

    // Test frustum and look-at
    test_frustum_and_look_at();

    printf("All tests completed.\n");
    return 0;
}
