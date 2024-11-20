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
vec4 eye = {0, 0, 3, 0};
vec4 at = {0, 0.0, 0, 0};
vec4 up = {0.0, 1, 0, 0.0};
GLuint use_texture_location;
GLuint use_diffuse_location;
GLuint use_ambient_location;
GLuint use_specular_location;
int use_texture = 1;
float far = -100;
float near = -1;
float left = -1;
float right = 1;
float top = 1;
float bottom = -1;
int use_ambient = 1;
int use_diffuse = 1;
int use_specular = 1;
void update()
{
    current_transformation_matrix = multiply_m4_m4(current_translation_matrix, multiply_m4_m4(current_rotation_matrix, current_scalar_matrix));
    glutPostRedisplay();
}
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
void make_cube(float x, float y, float z, int texture, float width)
{
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
    // Ambient lighting
    vec4 *ambient_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    for (int i = 0; i < num_vertices; i++)
    {
        ambient_colors[i] = scalar_v4(colors[i], .3);
    }

    // Diffuse lighting
    vec4 light_position = {5, 0, 4, 1};
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

    model_view = look_at(eye, at, up);
    projection = frustum(left, right, top, bottom, near, far);

    print_v4(colors[0]);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1, 0);
    // glUniform1i(glGetUniformLocation(program, "enable_specular"), enable_specular);
}
int isAnimating = 0;
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *)&model_view);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *)&projection);

    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&identity_ctm);
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
        isAnimating ^= 0x1;
    }
    else if (key == 'w')
    {
        isAnimating = 0;
        vec4 v = multiply_m4_vec4(current_rotation_matrix,(vec4){0,0,-.02,0});
        eye = add_v4(eye,v);
        at = add_v4(at,v);
        model_view = look_at(eye, at, up);
        glutPostRedisplay();
        return;
    }
     else if (key == 's')
    {
        isAnimating = 0;
        vec4 v = multiply_m4_vec4(current_rotation_matrix,(vec4){0,0,.02,0});
        eye = add_v4(eye,v);
        at = add_v4(at,v);
        model_view = look_at(eye, at, up);
        glutPostRedisplay();
        return;
    }
     else if (key == 'a')
    {
        isAnimating = 0;
        vec4 v = multiply_m4_vec4(current_rotation_matrix,(vec4){-.02,0,0,0});
        eye = add_v4(eye,v);
        at = add_v4(at,v);
        model_view = look_at(eye, at, up);
        glutPostRedisplay();
        return;
    }
    else if (key == 'd')
    {
        isAnimating = 0;
        vec4 v = multiply_m4_vec4(current_rotation_matrix,(vec4){.02,0,0,0});
        eye = add_v4(eye,v);
        at = add_v4(at,v);
        model_view = look_at(eye, at, up);
        glutPostRedisplay();
        return;
    }
    else if (key == 'e')
    {
        isAnimating = 0;
        eye = (vec4){-2.0, 1.0, 2.0, 1.0};
        at = (vec4){0.0, 0.0, 0.0, 1.0};
        up = (vec4){0.0, 1.0, 0.0, 0.0};
        model_view = look_at(eye, at, up);
        glutPostRedisplay();
    }
    else if(key == 't'){
        use_texture ^= 1;
        glUniform1i(use_texture_location, use_texture);
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
}

void mouse(int button, int state, int x, int y)
{
    ////printf("%i %i %i %i\n", button, state, x, y);
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
    eye = multiply_m4_vec4(rotation_matrix, eye);
    model_view = look_at(eye, at, up);
    last_touch = touch;
    touch = current_touch;
    glutPostRedisplay();
}

void idle()
{
    if (isAnimating)
    {
        x_angle += 0.1;
        y_angle += 0.2;
        identity_ctm = multiply_m4_m4(rotating_y_m4(x_angle),
                                      multiply_m4_m4(translation_m4(2, 0, 0), rotating_x_m4(y_angle)));
        z_angle -= 1;
        // glutPostRedisplay();
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
