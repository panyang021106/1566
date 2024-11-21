/*
 * template.c
 *
 * Created on: September 10, 2024
 * Author: Thumrongsak Kosiyatrakul
 */
#ifdef __APPLE__ // include Mac OS X verions of headers
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#endif // __APPLE__
#include "initShader.h"
#include "myLib.h" // replace this with your library
#include <stdio.h>
#include "mouse_lecture.h"
#include <stdbool.h>
#include "maze.h"
#include <math.h>
#define PI 3.14159
int num_vertices;
int current_state = 0;
float alpha = 0;
int current_look = 0;
mat4 identity_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 current_transformation_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 current_scalar_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 current_translation_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 current_rotation_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
vec4 touch = {0, 0, 0, 1};
vec4 previous_roatation = (vec4){0, 0, 0, 0};
Maze *maze;
int **maze_arr;
bool left_mouse = false;
GLuint ctm_location;
vec4 *positions;
vec4 *colors;
int mode = 0;
int current_vec;
vec2 *tex_coords;
vec4 *normals;
float offsets[7][2] = {{0, 0}, {.25, 0}, {.5, 0}, {.75, 0},{0,.25},{0,.5},{0,.75}};
vec4 cube[36];
GLfloat x_angle = 1.0;
GLfloat y_angle = 0.0;
GLfloat z_angle = 0.0;
mat4 model_view; // model-view matrix uniform shader variable location
GLuint projection_location;
GLuint model_view_location;
mat4 projection;
vec4 *ambient_colors;
vec4 eye = {0, 0, 2, 0};
vec4 at = {0, 0.0, 0, 0};
vec4 up = {0.0, 1, 0, 0.0};
vec4 og_eye = {0, 0, 2, 0};
vec4 og_at = {0, 0.0, 0, 0};
vec4 spotlight_direction = {0, 0, -1,0};
float spotlight_cutoff = 30;
float spotlight_exponent = 2;   
mat4 og_model_view;
mat4 og_projection;
GLuint use_texture_location;
GLuint use_diffuse_location;
GLuint use_ambient_location;
GLuint light_position_location;
GLuint use_specular_location;
GLuint use_flashlight_location;
GLuint spotlight_direction_location; 
GLuint spotlight_cutoff_location;
GLuint spotlight_position_location;
vec4 spotlight_position;
int use_texture = 1;
float far = -100;
float near = -.01;
float left = -.01;
float right = .01;
float top = .01;
float bottom = -.01;
int use_ambient = 1;
int use_diffuse = 1;
int use_specular = 1;
vec4 light_position = {0, 5, 0, 1};
float light_radius = 5.0;
float light_angle = 0.0;
int use_flashlight = 0;
int current_step = 0;
int current_i = -1;
int current_j = -1;
typedef enum {
    NONE = 0,
    FLYING_UP,
    FLYING_DOWN,
    WALK_FORWARD,
    WALK_BACKWARD,
    TURN_LEFT,
    TURN_RIGHT,
    LOOK_UP,
    LOOK_DOWN,
    TO_START,
    ENTER_MAZE,
    SLIDE_LEFT,
    SLIDE_RIGHT
} AnimationState;
void randomize_colors()
{
    colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    for (int i = 0; i < num_vertices - 2; i = i + 3)
    {
        float c1 = 0; //(float)rand()/(float)RAND_MAX;
        float c2 = 1; //(float)rand()/(float)RAND_MAX;
        float c3 = 0; //(float)rand()/(float)RAND_MAX;
        colors[i].x = c1;
        colors[i].y = c2;
        colors[i].z = c3;
        colors[i].w = 1.0;
        colors[i + 1].x = c1;
        colors[i + 1].y = c2;
        colors[i + 1].z = c3;
        colors[i + 1].w = 1.0;
        colors[i + 2].x = c1;
        colors[i + 2].y = c2;
        colors[i + 2].z = c3;
        colors[i + 2].w = 1.0;
    }
}

// texture one is top left, 2 is top row one to the left etc
void make_cube(float x, float z, float y, int texture, float width)
{
    y = y - .75;
    mat4 translation = translation_m4(x, y, z);
    float hw = width / 2;
    cube[0] = (vec4){hw, -hw, hw, 1.0};
    cube[1] = (vec4){-hw, hw, hw, 1.0};
    cube[2] = (vec4){-hw, -hw, hw, 1.0};
    cube[3] = (vec4){hw, -hw, hw, 1.0};
    cube[4] = (vec4){hw, hw, hw, 1.0};
    cube[5] = (vec4){-hw, hw, hw, 1.0};
    for (int i = 0; i < 6; i++)
    {
        normals[i + current_vec] = (vec4){0, 0, 1, 0};
    }
    cube[7] = (vec4){hw, -hw, -hw, 1.0};
    cube[6] = (vec4){-hw, hw, -hw, 1.0};
    cube[8] = (vec4){-hw, -hw, -hw, 1.0};
    cube[11] = (vec4){hw, -hw, -hw, 1.0};
    cube[10] = (vec4){hw, hw, -hw, 1.0};
    cube[9] = (vec4){-hw, hw, -hw, 1.0};
    for (int i = 6; i < 12; i++)
    {
        normals[i + current_vec] = (vec4){0, 0, -1, 0};
    }

    cube[12] = (vec4){hw, -hw, -hw, 1.0};
    cube[13] = (vec4){hw, hw, hw, 1.0};
    cube[14] = (vec4){hw, -hw, hw, 1.0};
    cube[15] = (vec4){hw, -hw, -hw, 1.0};
    cube[16] = (vec4){hw, hw, -hw, 1.0};
    cube[17] = (vec4){hw, hw, hw, 1.0};
    for (int i = 12; i < 18; i++)
    {
        normals[i + current_vec] = (vec4){1, 0, 0, 0};
    }
    cube[19] = (vec4){-hw, -hw, -hw, 1.0};
    cube[18] = (vec4){-hw, hw, hw, 1.0};
    cube[20] = (vec4){-hw, -hw, hw, 1.0};
    cube[23] = (vec4){-hw, -hw, -hw, 1.0};
    cube[22] = (vec4){-hw, hw, -hw, 1.0};
    cube[21] = (vec4){-hw, hw, hw, 1.0};
    for (int i = 18; i < 24; i++)
    {
        normals[i + current_vec] = (vec4){-1, 0, 0, 0};
    }
    cube[25] = (vec4){hw, hw, -hw, 1.0};
    cube[24] = (vec4){-hw, hw, hw, 1.0};
    cube[26] = (vec4){-hw, hw, -hw, 1.0};
    cube[29] = (vec4){hw, hw, -hw, 1.0};
    cube[28] = (vec4){hw, hw, hw, 1.0};
    cube[27] = (vec4){-hw, hw, hw, 1.0};
    for (int i = 24; i < 30; i++)
    {
        normals[i + current_vec] = (vec4){0, 1, 0, 0};
    }
    cube[30] = (vec4){hw, -hw, -hw, 1.0};
    cube[31] = (vec4){-hw, -hw, hw, 1.0};
    cube[32] = (vec4){-hw, -hw, -hw, 1.0};
    cube[33] = (vec4){hw, -hw, -hw, 1.0};
    cube[34] = (vec4){hw, -hw, hw, 1.0};
    cube[35] = (vec4){-hw, -hw, hw, 1.0};
    for (int i = 30; i < 36; i++)
    {
        normals[i + current_vec] = (vec4){0, -1, 0, 0};
    }
    for (int i = 0; i < 36; i++)
    {
        positions[current_vec + i] = multiply_m4_vec4(translation, cube[i]);
    }

    for (int i = 0; i < 6; i++)
    {
        int offset = current_vec + i * 6;
        float x0 = offsets[texture][0];
        float y0 = offsets[texture][1];
        float x1 = x0 + .25;
        float y1 = y0 + .25;

        tex_coords[offset + 0] = (vec2){x1, y1};
        tex_coords[offset + 1] = (vec2){x0, y0};
        tex_coords[offset + 2] = (vec2){x0, y1};
        tex_coords[offset + 3] = (vec2){x1, y1};
        tex_coords[offset + 4] = (vec2){x1, y0};
        tex_coords[offset + 5] = (vec2){x0, y0};
    }
    current_vec = current_vec + 36;
}
float cube_size = 1.5 / MAZE_SIZE;
void make_base()
{
    for (int y = (int)(MAZE_SIZE / 2 + 1); y >= 0; y--)
    {
        for (int i = -y; i < y; i++)
        {
            for (int j = -y; j < y; j++)
            {
                if (rand() % 100 < 60)
                    make_cube(cube_size / 2 + i * cube_size, cube_size / 2 + j * cube_size, y * cube_size - (int)(MAZE_SIZE / 2 + 1) * cube_size + .5, 6, cube_size);
            }
        }
    }
}
float maze_start_x;
float maze_start_z;
void make_maze()
{
    for (int i = 0; i < MAZE_SIZE; i++)
    {
        for (int j = 0; j < MAZE_SIZE; j++)
        {
            if (maze_arr[i][j] == 1)
            {
                make_cube(-.75 + cube_size / 2 + i * cube_size, -.75 + cube_size / 2 + j * cube_size, .5 + cube_size, 2, cube_size);
                make_cube(-.75 + cube_size / 2 + i * cube_size, -.75 + cube_size / 2 + j * cube_size, .5 + cube_size * 2, 2, cube_size);
                if (rand() % 100 < 60)
                make_cube(-.75 + cube_size / 2 + i * cube_size, -.75 + cube_size / 2 + j * cube_size, .5 + cube_size * 3, 2, cube_size);
            }
            if (maze_arr[i][j] == 2)
            {
                make_cube(-.75 + cube_size / 2 + i * cube_size, -.75 + cube_size / 2 + j * cube_size, .5 + cube_size, 4, cube_size);
                make_cube(-.75 + cube_size / 2 + i * cube_size, -.75 + cube_size / 2 + j * cube_size, .5 + cube_size * 2, 4, cube_size);
                if (rand() % 100 < 60)
                make_cube(-.75 + cube_size / 2 + i * cube_size, -.75 + cube_size / 2 + j * cube_size, .5 + cube_size * 3, 4, cube_size);
            }
            else
            {
                make_cube(-.75 + cube_size / 2 + i * cube_size, -.75 + cube_size / 2 + j * cube_size, .5 + cube_size, 0, cube_size);
            }
            if(i == 1 && j ==1){
                maze_start_x = -.75 + cube_size / 2 + i * cube_size;
                maze_start_z =  -.75 + cube_size / 2 + j * cube_size;
            }
        }
    }
}

vec4 calculate_point(float x, float y)
{
    float z;
    if (x * x + y * y >= 1)
    {
        return (vec4){0, 0, 0, 1};
    }
    z = sqrt(1 - x * x - y * y);
    vec4 a = (vec4){x, y, z, 0};
    // //printf("%.6f",magnitude_v4(a));
    // printf("%.6f,%.6f",x,y);
    return scalar_v4(a, 1 / magnitude_v4(a));
}

void setup_lighting()
{
    return;
    // Ambient lighting
    vec4 *ambient_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    for (int i = 0; i < num_vertices; i++)
    {
        ambient_colors[i] = scalar_v4(colors[i], .3);
    }

    // Diffuse lighting
    vec4 *diffuse_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    for (int i = 0; i < num_vertices; i++)
    {
        vec4 l = normalize_v4(sub_v4(light_position, positions[i]));
        vec4 n = normals[i];
        float l_dot_n = dot_v4(l, n);
        if (l_dot_n < 0)
            l_dot_n = 0;
        diffuse_colors[i] = scalar_v4(colors[i], l_dot_n);
    }

    // Specular lighting
    vec4 *specular_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    for (int i = 0; i < num_vertices; i++)
    {
        vec4 l = normalize_v4(sub_v4(light_position, positions[i]));
        vec4 v = normalize_v4(sub_v4(eye, positions[i]));
        vec4 h = normalize_v4(add_v4(l, v));
        vec4 n = normals[i];
        float h_dot_n = dot_v4(h, n);
        if (h_dot_n < 0)
            h_dot_n = 0;
        specular_colors[i] = scalar_v4((vec4){1, 1, 1, 1}, pow(h_dot_n, 50));
    }

    // Combine ambient, diffuse, and specular lighting
    for (int i = 0; i < num_vertices; i++)
    {
        colors[i] = add_v4(specular_colors[i], add_v4(diffuse_colors[i], ambient_colors[i]));
    }
}
vec4 entrance_at;
vec4 entrance_eye;

void init(void)
{

    maze = (Maze *)malloc(sizeof(Maze));
    maze_generate(maze);
    print_maze(maze);
    maze_arr = maze_array(maze);

    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    num_vertices = 100000;
    positions = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    tex_coords = (vec2 *)malloc(sizeof(vec2) * num_vertices);
    normals = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    ambient_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    make_base();
    make_maze();
    entrance_eye = (vec4){-maze_start_x,0,-maze_start_z + cube_size,0};
    entrance_at = (vec4){-maze_start_x,0,-maze_start_z - cube_size,0};

    randomize_colors();

    int tex_width = 64;
    int tex_height = 64;
    GLubyte my_texels[tex_width][tex_height][3];

    FILE *fp = fopen("textures01.raw", "r");
    if (fp == NULL)
    {
        printf("[textureTemplate] Successfully open a texture file.\n");
        exit(0);
    }
    fread(my_texels, tex_width * tex_height * 3, 1, fp);
    fclose(fp);

    setup_lighting();

    glUseProgram(program);

    GLuint mytex[1];
    glGenTextures(1, mytex);
    glBindTexture(GL_TEXTURE_2D, mytex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texels);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    int param;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);

    GLuint vao;
#ifdef __APPLE__
    glGenVertexArraysAPPLE(1, &vao);
    glBindVertexArrayAPPLE(vao);
#else
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
#endif

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * 3 * num_vertices + sizeof(vec2) * num_vertices, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * num_vertices, positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices, sizeof(vec4) * num_vertices, colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * 2 * num_vertices, sizeof(vec4) * num_vertices, normals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * 3 * num_vertices, sizeof(vec2) * num_vertices, tex_coords);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(vec4) * num_vertices));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(vec4) * 2 * num_vertices));

    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(vec4) * 3 * num_vertices));

    GLuint spotlight_direction_location = glGetUniformLocation(program, "spotlight_direction");
    spotlight_direction = at;
    glUniform4fv(spotlight_direction_location, 1, (GLfloat *)&spotlight_direction);

    GLuint spotlight_position_location = glGetUniformLocation(program, "spotlight_position");
    spotlight_position = eye;
    glUniform4fv(spotlight_position_location,1, (GLfloat *)&spotlight_position);

    GLuint spotlight_cutoff_location = glGetUniformLocation(program, "spotlight_cutoff");
    glUniform1f(spotlight_cutoff_location, spotlight_cutoff);

    GLuint spotlight_exponent_location = glGetUniformLocation(program, "spotlight_exponent");
    glUniform1f(spotlight_exponent_location, spotlight_exponent);
    ctm_location = glGetUniformLocation(program, "ctm");
    model_view_location = glGetUniformLocation(program, "model_view");
    projection_location = glGetUniformLocation(program, "projection");

    use_texture_location = glGetUniformLocation(program, "use_texture");
    glUniform1i(use_texture_location, use_texture);


    use_ambient_location = glGetUniformLocation(program, "use_ambient");
    glUniform1i(use_ambient_location, use_ambient);

    use_diffuse_location = glGetUniformLocation(program, "use_diffuse");
    glUniform1i(use_diffuse_location, use_diffuse);

    use_specular_location = glGetUniformLocation(program, "use_specular");
    glUniform1i(use_specular_location, use_specular);

    light_position_location = glGetUniformLocation(program,
    "light_position");
    glUniform4fv(light_position_location, 1, (GLvoid *) &light_position);
   
    use_flashlight_location = glGetUniformLocation(program, "use_spotlight");
    glUniform1i(use_flashlight_location, use_flashlight);
    model_view = look_at(eye, at, up);
    projection = frustum(left, right, bottom, top, near, far);
    og_model_view = model_view;
    og_projection = projection;

    print_v4(colors[0]);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1, 0);
    // glUniform1i(glGetUniformLocation(program, "enable_specular"), enable_specular);
}
int is_animating = 0;
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *)&model_view);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *)&projection);
    glUniform4fv(light_position_location, 1, (GLfloat *)&light_position);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&current_transformation_matrix);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if (key == 'q')
    {
#ifndef __APPLE__
        exit(0);
#else
        glutLeaveMainLoop();
#endif
    }
    else if (key == ' ')
    {
        is_animating ^= 0x1;
    }
    else if (key == 'w')
    {
        if(is_animating) return;
        is_animating = 1;
        current_state = WALK_FORWARD;
    }
     else if (key == 's')
    {
        if(is_animating) return;
        is_animating = 1;
        current_state = WALK_BACKWARD;
        current_step = 0;
    }
     else if (key == 'a')
    {
        if(is_animating) return;
        is_animating = 1;
        current_state = SLIDE_LEFT;
        current_step = 0;
    }
    else if (key == 'd')
    {
        if(is_animating) return;
        current_step = 0;
        is_animating = 1;
        current_state = SLIDE_RIGHT;
    }
    else if (key == 'e')
    {
        if(is_animating) return;
        current_step = 0;
        is_animating = 1;
        current_state = TO_START;
    }
        else if (key == '9')
    {
        if(is_animating) return;
        current_step = 0;
        is_animating = 1;
        current_state = TURN_LEFT;
    }    else if (key == '0')
    {
        if(is_animating) return;
        current_step = 0;
        is_animating = 1;
        current_state = TURN_RIGHT;
    }
    else if(key == 't'){

        use_texture ^= 1;
        glUniform1i(use_texture_location, use_texture);
    }
    else if (key == 'n')
    {
        is_animating = 0;
    
        vec4 direction = sub_v4(at, eye);
        mat4 rotation = rotating_y_m4(-5.0); 
        vec4 rotated_direction = multiply_m4_vec4(rotation, direction);
        at = add_v4(eye, rotated_direction);
        model_view = look_at(eye, at, up);
        glutPostRedisplay();

    }
     else if (key == 'm')
    {
        is_animating = 0;
    vec4 direction = sub_v4(at, eye);
    mat4 rotation = rotating_y_m4(5.0); 
    vec4 rotated_direction = multiply_m4_vec4(rotation, direction);
    at = add_v4(eye, rotated_direction);
    model_view = look_at(eye, at, up);
    glutPostRedisplay();
    }
    else if(key == 'y'){
        use_ambient ^= 1;
        glUniform1i(use_ambient_location, use_ambient);
    }    else if(key == 'u'){
        use_diffuse ^= 1;
        glUniform1i(use_diffuse_location, use_diffuse);
    }    else if(key == 'i'){
        use_specular ^= 1;
        glUniform1i(use_specular_location, use_specular);
    }
    else if(key == '1'){
        current_vec = 0;
        light_angle += .2;
        light_position = (vec4){0,light_radius * cos(light_angle), light_radius * sin(light_angle),1};
        glutPostRedisplay();
    }
    else if(key == '2'){
        light_angle -= .2;
        light_position = (vec4){0,light_radius * cos(light_angle), light_radius * sin(light_angle),1};
        glutPostRedisplay();
    }
    else if(key == 'f'){
        if(use_flashlight){
            light_position = eye;
        }
        else{
            light_angle = 0;
            light_position = (vec4){0,light_radius * cos(light_angle), light_radius * sin(light_angle),1};
        }
        use_flashlight^= 1;
        glUniform1i(use_flashlight_location, use_flashlight);
    }
    else if(key == 'x'){
        if(is_animating)return;
        is_animating = 1;
        current_step = 0;
        current_state = ENTER_MAZE;
    }
}
void mouse(int button, int state, int x, int y)
{
    ///printf("%i %i %i %i\n", button, state, x, y);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        float x_coordinate = (x * 2.0 / 511.0) - 1;
        float y_coordinate = -(y * 2.0 / 511.0) + 1;
        if (x_coordinate * x_coordinate + y_coordinate * y_coordinate > 1)
        {
            return;
        }

        // current_translation_matrix = translation_m4(x_coordinate, y_coordinate, 0);
        touch = calculate_point(x_coordinate, y_coordinate);
        previous_roatation = touch;
        left_mouse = true;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        float x_coordinate = (x * 2.0 / 511.0) - 1;
        float y_coordinate = -(y * 2.0 / 511.0) + 1;

        left_mouse = false;
    }
    // if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    //     current_scalar_matrix = scaling_m4(.5,.5,.5);
    // }
    // if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
    //     current_scalar_matrix = scaling_m4(1,1,1);
    // }
    if (button == 3) {
        left *= 0.9f;
        right *= 0.9f;
        bottom *= 0.9f;
        top *= 0.9f;
    }
    else if (button == 4) {
        left *= 1.1f;
        right *= 1.1f;
        bottom *= 1.1f;
        top *= 1.1f;
    }
    projection= frustum(left,right,bottom,top,near,far);
    glutPostRedisplay();
}

mat4 rotation_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
vec4 last_touch = {0, 0, 0, 0};
bool last_touch_exists = false;
void motion(int x, int y)
{
    float x_coordinate = (x * 2.0f / 511.0f) - 1.0f;
    float y_coordinate = -(y * 2.0f / 511.0f) + 1.0f;

    if (x_coordinate * x_coordinate + y_coordinate * y_coordinate > 1.0f)
    {
        return;
    }

    vec4 current_touch = calculate_point(x_coordinate, y_coordinate);

    vec4 about_vector = cross_product_v4(current_touch, touch);
    about_vector = scalar_v4(about_vector, 1 / magnitude_v4(about_vector));

    float dot = dot_v4(touch, current_touch);
    float deg = acos(dot);

    rotation_matrix = rotation_axis_angle_m4(about_vector, deg);
    current_rotation_matrix = multiply_m4_m4(rotation_matrix, current_rotation_matrix);
    current_transformation_matrix = current_rotation_matrix;
    model_view = look_at(eye, at, up);
    last_touch = touch;
    touch = current_touch;
    glutPostRedisplay();
}
bool valid_move(int i, int j){
    //make_cube(-.75 + cube_size / 2 + i * cube_size, -.75 + cube_size / 2 + j * cube_size, .5 + cube_size, 0, cube_size);
    printf("%d,%d\n",i,j);
    return maze_arr[i][j] ==0;

//     entrance_eye = (vec4){-maze_start_x,cube_size,-maze_start_z + cube_size,0};
//     if maze_arr
}

void idle()
{
    if (is_animating)
    {
        if(current_state == TO_START){
            current_step++;
            alpha = (float)current_step /100;
            current_transformation_matrix = add_m4(current_transformation_matrix,scalar_m4(identity_ctm,alpha));
            model_view = add_m4(model_view,scalar_m4(og_model_view,alpha));
            projection = add_m4(projection,scalar_m4(og_projection,alpha));
 
            if(current_step == 100){
                current_rotation_matrix = identity_ctm;
                eye = og_eye;
                at = og_at;
                current_step = 0;
                is_animating = 0;
                projection = og_projection;
                model_view = og_model_view;
                current_transformation_matrix = identity_ctm;
                current_i = -1;
                current_j = -1;
            }
        }
        if(current_state == ENTER_MAZE){
            current_step++;
            alpha = (float)current_step /100;
            mat4 m = look_at(entrance_eye,entrance_at,up);
            // current_transformation_matrix = add_m4(current_transformation_matrix,scalar_m4(identity_ctm,alpha));
            model_view = add_m4(model_view,scalar_m4(m,alpha));
            // projection = add_m4(projection,scalar_m4(og_projection,alpha));
            if(current_step == 100){
                current_rotation_matrix = identity_ctm;
                eye = entrance_eye;
                at = entrance_at;
                current_step = 0;
                is_animating = 0;
                projection = og_projection;
                current_transformation_matrix = identity_ctm;
                model_view = look_at(entrance_eye,entrance_at,up);
                current_i = MAZE_SIZE - 2;
                current_j = MAZE_SIZE - 1;
            }
        }


        if(current_state == WALK_FORWARD || current_state ==  WALK_BACKWARD || current_state ==  SLIDE_LEFT ||current_state ==  SLIDE_RIGHT){
            current_step++;
            alpha = (float)current_step /100;
            float z_dif = 0;
            float x_dif = 0;
            int ti = current_i;
            int tj = current_j;
            if(current_state == WALK_FORWARD && current_look == 0 || current_state == SLIDE_RIGHT && current_look == 1 || current_state == WALK_BACKWARD && current_look == 2 || current_state == SLIDE_LEFT && current_look == 3){
                z_dif = cube_size;
                tj -= 1;
            }
            if(current_state == WALK_FORWARD && current_look == 2 || current_state == SLIDE_RIGHT && current_look == 3 || current_state == WALK_BACKWARD && current_look == 0 || current_state == SLIDE_LEFT && current_look == 1){
                z_dif = -cube_size;
                tj += 1;
            }
            if(current_state == WALK_FORWARD && current_look == 1 || current_state == SLIDE_RIGHT && current_look == 2 || current_state == WALK_BACKWARD && current_look == 3 || current_state == SLIDE_LEFT && current_look == 0){
                x_dif = cube_size;
                ti -= 1;
            }
            if(current_state == WALK_FORWARD && current_look == 3 || current_state == SLIDE_RIGHT && current_look == 0 || current_state == WALK_BACKWARD && current_look == 1 || current_state == SLIDE_LEFT && current_look == 2){
                x_dif = -cube_size;
                ti += 1;
            }

            vec4 next_eye = (vec4){eye.x - x_dif,eye.y,eye.z - z_dif,eye.w};
            if(!valid_move(ti,tj)){
                current_step = 0;
                is_animating = 0;
                return;
            }
            vec4 next_at = (vec4){at.x - x_dif,at.y,at.z - z_dif,eye.w};
            mat4 m = look_at(next_eye,next_at,up);
            // current_transformation_matrix = add_m4(current_transformation_matrix,scalar_m4(identity_ctm,alpha));
            model_view = add_m4(model_view,scalar_m4(m,alpha));
            // projection = add_m4(projection,scalar_m4(og_projection,alpha));
            if(current_step == 100){
                current_i = ti;
                current_j = tj;
                current_rotation_matrix = identity_ctm;
                eye = next_eye;
                at = next_at;
                current_step = 0;
                is_animating = 0;
                projection = og_projection;
                current_transformation_matrix = identity_ctm;
                model_view = look_at(next_eye,next_at,up);
            }

        }

        if(current_state == TURN_LEFT || current_state == TURN_RIGHT){
            float c = 1;
            if(current_state == TURN_RIGHT){
                c = -1;
            }
            current_step++;
            alpha = (float)current_step /100;
            float z_dif = 0;
            float x_dif = 0;
            if(current_look ==0)
                x_dif = 2 * c * cube_size;
            if(current_look ==2)
                x_dif = -2 * c * cube_size;
            if(current_look ==3)
                z_dif = 2 * c * cube_size;
            if(current_look ==1)
                z_dif = -2 * c * cube_size;
            vec4 next_eye = eye;
            vec4 next_at = (vec4){eye.x - x_dif,eye.y,eye.z - z_dif,eye.w};
            mat4 m = look_at(next_eye,next_at,up);
            // current_transformation_matrix = add_m4(current_transformation_matrix,scalar_m4(identity_ctm,alpha));
            model_view = add_m4(model_view,scalar_m4(m,alpha));
            // projection = add_m4(projection,scalar_m4(og_projection,alpha));
            if(current_step == 100){
                current_look = (current_look + (int)c) % 4;
                if (current_look == -1) current_look = 3;
                current_rotation_matrix = identity_ctm;
                eye = next_eye;
                at = next_at;
                current_step = 0;
                is_animating = 0;
                projection = og_projection;
                current_transformation_matrix = identity_ctm;
                model_view = look_at(next_eye,next_at,up);
            }
        }
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Mouse Lecture");
#ifndef __APPLE__
    glewInit();
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
