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
#include <math.h>
#include "maze.h"
#define PI  3.14159
int num_vertices;
mat4 identity = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
mat4 current_transformation_matrix = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
mat4 current_scalar_matrix = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
mat4 current_translation_matrix = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
vec4 touch = {0,0,0,1};
vec4 previous_roatation = (vec4){0,0,0,0};
Maze *maze; 
bool left_mouse = false;
GLuint ctm_location;
vec4 *positions;
vec4 *colors;
int mode = 0;
int current_vec;
vec2 *tex_coords;
float offsets[4][2] = {{0, 0},{.25, 0},{.5, 0},{.75, 0}};
vec4 cube[36];
void randomize_colors(){
    colors = (vec4 *) malloc(sizeof(vec4) * num_vertices);
    for(int i = 0; i < num_vertices - 2; i = i + 3){
            float c1 = (float)rand()/(float)RAND_MAX;
            float c2 = (float)rand()/(float)RAND_MAX;
            float c3 = (float)rand()/(float)RAND_MAX;
            colors[i].x = c1; colors[i].y = c2; colors[i].z = c3; colors[i].w = 1.0;
            colors[i + 1].x = c1; colors[i + 1].y = c2; colors[i + 1].z = c3; colors[i + 1].w = 1.0;
            colors[i + 2].x = c1; colors[i + 2].y = c2; colors[i + 2].z = c3; colors[i + 2].w = 1.0;    
    }
}

//texture one is top left, 2 is top row one to the left etc
void make_cube(float x, float y,float z, int texture, float width){
    mat4 translation = translation_m4(x, y, z);
    float hw = width / 2;
   cube[0] = (vec4){  hw, -hw,  hw, 1.0 };
    cube[1] = (vec4){ -hw,  hw,  hw, 1.0 };
    cube[2] = (vec4){ -hw, -hw,  hw, 1.0 };
    cube[3] = (vec4){  hw, -hw,  hw, 1.0 };
    cube[4] = (vec4){  hw,  hw,  hw, 1.0 };
    cube[5] = (vec4){ -hw,  hw,  hw, 1.0 };

    cube[6]  = (vec4){  hw, -hw, -hw, 1.0 };
    cube[7]  = (vec4){ -hw,  hw, -hw, 1.0 };
    cube[8]  = (vec4){ -hw, -hw, -hw, 1.0 };
    cube[9]  = (vec4){  hw, -hw, -hw, 1.0 };
    cube[10] = (vec4){  hw,  hw, -hw, 1.0 };
    cube[11] = (vec4){ -hw,  hw, -hw, 1.0 };

    cube[12] = (vec4){  hw, -hw, -hw, 1.0 };
    cube[13] = (vec4){  hw,  hw,  hw, 1.0 };
    cube[14] = (vec4){  hw, -hw,  hw, 1.0 };
    cube[15] = (vec4){  hw, -hw, -hw, 1.0 };
    cube[16] = (vec4){  hw,  hw, -hw, 1.0 };
    cube[17] = (vec4){  hw,  hw,  hw, 1.0 };
    
    cube[18] = (vec4){ -hw, -hw, -hw, 1.0 };
    cube[19] = (vec4){ -hw,  hw,  hw, 1.0 };
    cube[20] = (vec4){ -hw, -hw,  hw, 1.0 };
    cube[21] = (vec4){ -hw, -hw, -hw, 1.0 };
    cube[22] = (vec4){ -hw,  hw, -hw, 1.0 };
    cube[23] = (vec4){ -hw,  hw,  hw, 1.0 };

    cube[24] = (vec4){  hw,  hw, -hw, 1.0 };
    cube[25] = (vec4){ -hw,  hw,  hw, 1.0 };
    cube[26] = (vec4){ -hw,  hw, -hw, 1.0 };
    cube[27] = (vec4){  hw,  hw, -hw, 1.0 };
    cube[28] = (vec4){  hw,  hw,  hw, 1.0 };
    cube[29] = (vec4){ -hw,  hw,  hw, 1.0 };

    cube[30] = (vec4){  hw, -hw, -hw, 1.0 };
    cube[31] = (vec4){ -hw, -hw,  hw, 1.0 };
    cube[32] = (vec4){ -hw, -hw, -hw, 1.0 };
    cube[33] = (vec4){  hw, -hw, -hw, 1.0 };
    cube[34] = (vec4){  hw, -hw,  hw, 1.0 };
    cube[35] = (vec4){ -hw, -hw,  hw, 1.0 };

    for(int i = 0; i < 36; i ++){
        positions[current_vec + i] = multiply_m4_vec4(translation,cube[i]);
    }
    
    for (int i = 0; i < 6; i++) {
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
void init(void)
{
    maze = (Maze*)malloc(sizeof(Maze));
    maze_generate(maze);
    print_maze(maze);

    current_vec = 0;
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    num_vertices = 72;
    positions = (vec4 *) malloc(sizeof(vec4) * num_vertices);
    tex_coords = (vec2 *) malloc(sizeof(vec2) * num_vertices);
    make_cube(.5,.75,0,0,.5);
    make_cube(.5,.25,0,1,.5);


    randomize_colors();

    
    
    int tex_width = 64;
    int tex_height = 64;
    GLubyte my_texels[tex_width][tex_height][3];
    FILE *fp = fopen("textures01.raw", "r");
    fread(my_texels, tex_width * tex_height * 3, 1, fp);
    fclose(fp);

    GLuint mytex[1];
    glGenTextures(1, mytex);
    glBindTexture(GL_TEXTURE_2D, mytex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB,
    GL_UNSIGNED_BYTE, my_texels);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    int param;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);

    GLuint vao;
    #ifndef __APPLE_
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    #else
    glGenVertexArraysAPPLE(1, &vao);
    glBindVertexArrayAPPLE(vao);
    #endif
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)  * num_vertices  + sizeof(vec4) * num_vertices + sizeof(vec2) * num_vertices, NULL,
    GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * num_vertices, positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices, sizeof(vec4) * num_vertices,colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices + sizeof(vec4) * num_vertices, sizeof(vec2) * num_vertices,tex_coords);
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (0));
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)
    (sizeof(vec4) * num_vertices));
    GLuint vTexCoord = glGetAttribLocation(program,"vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord,2,GL_FLOAT,GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * num_vertices  + sizeof(vec4) * num_vertices));
    GLuint texture_location = glGetUniformLocation(program, "texture");
    glUniform1i(texture_location, 0);
    ctm_location = glGetUniformLocation(program, "ctm");
    // glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}
void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)
    &current_transformation_matrix);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    glutSwapBuffers();
}
void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q') {
        #ifndef __APPLE__
        glutLeaveMainLoop();
        #else
        exit(0);
        #endif
    }
    if(key == '1'){
        if (mode == 0)return;
        mode = 0;
        init();
    }
    if (key == '2'){
        if (mode == 1)return;
        mode = 1;
        init();
    }
    if (key == '3'){
        if (mode == 2)return;
        mode = 2;
        init();
    }
    if (key == '4'){
        if (mode == 7)return;
        mode = 7;
        init();
    }
    if (key == '5'){
        if (mode == 8)return;
        mode = 8;
        init();
    }
    if (key == '6'){
        if (mode == 9)return;
        mode = 9;
        init();
    }
    if(key == 'r'){
        if(mode == 4) return;
        mode = 4;
        init();
    }
    // else if(key == 'u')
    // current_transformation_matrix = rotate_x(-45);
    // else if(key == 'd')
    // current_transformation_matrix = rotate_x(45);
    // else if(key == ' ')
    // current_transformation_matrix = m4_identity();
    glutPostRedisplay();
}
//radius = 2, so no point is out of the sphere
vec4 calculate_point(float x, float y){
    float z;
    if(x * x + y * y >= 1){
        return (vec4){0,0,0,1};
    }
    z = sqrt(1 - x * x - y * y);
    vec4 a = (vec4){x,y,z,0};
    // //printf("%.6f",magnitude_v4(a));
    //printf("%.6f,%.6f",x,y);
    return scalar_v4(a,1 / magnitude_v4(a));
}
void mouse(int button, int state, int x, int y) {
    ////printf("%i %i %i %i\n", button, state, x, y);
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float x_coordinate = (x * 2.0 / 511.0) - 1;
        float y_coordinate = -(y * 2.0 / 511.0) + 1;
            if(x_coordinate * x_coordinate + y_coordinate * y_coordinate > 1){
            return;
         }

        //current_translation_matrix = translation_m4(x_coordinate, y_coordinate, 0);
        touch = calculate_point(x_coordinate,y_coordinate);
        previous_roatation = touch;
        left_mouse = true;
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
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
        current_scalar_matrix = multiply_m4_m4(scaling_m4(1.02, 1.02, 1.02),current_scalar_matrix); 
    }
    else if (button == 4) { // Scroll down: zoom out
        current_scalar_matrix = multiply_m4_m4(scaling_m4(1/ 1.02, 1 /1.02, 1 / 1.02),current_scalar_matrix); 
    }
    current_transformation_matrix = multiply_m4_m4(current_translation_matrix,current_scalar_matrix);
    glutPostRedisplay();
}


mat4 rotation_matrix = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
vec4 last_touch = {0,0,0,0};
bool last_touch_exists = false;
void motion(int x, int y) {
    float x_coordinate = (x * 2.0f / 511.0f) - 1.0f;
    float y_coordinate = -(y * 2.0f / 511.0f) + 1.0f;
    
    if (x_coordinate * x_coordinate + y_coordinate * y_coordinate > 1.0f) {
        return;
    }   

    vec4 current_touch = calculate_point(x_coordinate, y_coordinate);

    vec4 about_vector = cross_product_v4(current_touch, touch);
    about_vector = scalar_v4(about_vector, 1 /  magnitude_v4(about_vector));

    float dot = dot_v4(touch, current_touch);
    float deg = acos(dot); 

    rotation_matrix = rotation_axis_angle_m4(about_vector, deg);
    current_translation_matrix = multiply_m4_m4(rotation_matrix, current_translation_matrix);
    current_transformation_matrix = multiply_m4_m4(current_translation_matrix, current_scalar_matrix);
    last_touch = touch;
    touch = current_touch;

    glutPostRedisplay();
}

void idle(){
    if(!last_touch_exists) return;
    vec4 about_vector = cross_product_v4(last_touch, touch);
    about_vector = scalar_v4(about_vector, 1 /  magnitude_v4(about_vector));

    float dot = dot_v4(touch, last_touch);
    float deg = acos(dot); 

    rotation_matrix = rotation_axis_angle_m4(about_vector, deg);
    current_translation_matrix = multiply_m4_m4(rotation_matrix, current_translation_matrix);
    current_transformation_matrix = multiply_m4_m4(current_translation_matrix, current_scalar_matrix);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
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
