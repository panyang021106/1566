//
// Created by 潘阳 on 24-11-18.
//
// Lighting-related functions extracted from lighting.c

// Set up ambient, diffuse, and specular lighting
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct {
    float v[4];
} vec4;

// Define struct for mat4
typedef struct {
    float m[4][4];
} mat4;

vec4 v4v4_addition(vec4 a, vec4 b) {
    vec4 result;
    for (int i = 0; i < 4; i++) {
        result.v[i] = a.v[i] + b.v[i];
    }
    return result;
}

vec4 v4v4_subtraction(vec4 a, vec4 b) {
    vec4 result;
    for (int i = 0; i < 4; i++) {
        result.v[i] = a.v[i] - b.v[i];
    }
    return result;
}

vec4 sv4_multiplication(float scalar, vec4 v) {
    vec4 result;
    for (int i = 0; i < 4; i++) {
        result.v[i] = scalar * v.v[i];
    }
    return result;
}

float v4v4_dot(vec4 a, vec4 b) {
    float result = 0.0;
    for (int i = 0; i < 3; i++) { // Assuming we're using the first three components
        result += a.v[i] * b.v[i];
    }
    return result;
}

vec4 v4_normalize(vec4 v) {
    float magnitude = 0.0;
    for (int i = 0; i < 3; i++) {
        magnitude += v.v[i] * v.v[i];
    }
    magnitude = sqrt(magnitude);
    vec4 result;
    for (int i = 0; i < 3; i++) {
        result.v[i] = v.v[i] / magnitude;
    }
    result.v[3] = v.v[3]; // Keep the w component the same
    return result;
}

void setup_lighting(vec4 *colors, vec4 *positions, vec4 *normals, int num_vertices, vec4 eye) {
    // Ambient lighting
    vec4 *ambient_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    for (int i = 0; i < num_vertices; i++) {
        ambient_colors[i] = sv4_multiplication(0.3, colors[i]);
    }

    // Diffuse lighting
    vec4 light_position = {4, 4, 4, 1};
    vec4 *diffuse_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    for (int i = 0; i < num_vertices; i++) {
        vec4 l = v4_normalize(v4v4_subtraction(light_position, positions[i]));
        vec4 n = normals[i];
        float l_dot_n = v4v4_dot(l, n);
        if (l_dot_n < 0) l_dot_n = 0;
        diffuse_colors[i] = sv4_multiplication(l_dot_n, colors[i]);
    }

    // Specular lighting
    vec4 *specular_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    for (int i = 0; i < num_vertices; i++) {
        vec4 l = v4_normalize(v4v4_subtraction(light_position, positions[i]));
        vec4 v = v4_normalize(v4v4_subtraction(eye, positions[i]));
        vec4 h = v4_normalize(v4v4_addition(l, v));
        vec4 n = normals[i];
        float h_dot_n = v4v4_dot(h, n);
        if (h_dot_n < 0) h_dot_n = 0;
        specular_colors[i] = sv4_multiplication(pow(h_dot_n, 50), (vec4){1, 1, 1, 1});
    }

    // Combine ambient, diffuse, and specular lighting
    for (int i = 0; i < num_vertices; i++) {
        colors[i] = v4v4_addition(specular_colors[i], v4v4_addition(diffuse_colors[i], ambient_colors[i]));
    }

    free(ambient_colors);
    free(diffuse_colors);
    free(specular_colors);
}

// Example usage of setup_lighting function
void init_lighting_example() {
    // Assuming we have already generated sphere_positions, sphere_colors, and sphere_normals
    vec4 sphere_positions[] = {
        {0.0, 0.0, 1.0, 1.0}, {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0},
        {0.0, 0.0, -1.0, 1.0}, {-1.0, 0.0, 0.0, 1.0}, {0.0, -1.0, 0.0, 1.0}
    };
    vec4 sphere_normals[] = {
        {0.0, 0.0, 1.0, 0.0}, {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, -1.0, 0.0}, {-1.0, 0.0, 0.0, 0.0}, {0.0, -1.0, 0.0, 0.0}
    };
    vec4 sphere_colors[] = {
        {0.6, 0.3, 0.2, 1.0}, {0.6, 0.3, 0.2, 1.0}, {0.6, 0.3, 0.2, 1.0},
        {0.6, 0.3, 0.2, 1.0}, {0.6, 0.3, 0.2, 1.0}, {0.6, 0.3, 0.2, 1.0}
    };
    int sphere_num_vertices = 6;
    vec4 eye = {0.0, 0.0, 5.0, 1.0};

    setup_lighting(sphere_colors, sphere_positions, sphere_normals, sphere_num_vertices, eye);

    // The colors array now contains the final lighting values for each vertex
    for (int i = 0; i < sphere_num_vertices; i++) {
        printf("Vertex %d - Color: (%f, %f, %f, %f)\n", i, sphere_colors[i].v[0], sphere_colors[i].v[1], sphere_colors[i].v[2], sphere_colors[i].v[3]);
    }
}

// Main function to run the lighting test
int main() {
    init_lighting_example();
    return 0;
}
