
#ifdef __APPLE__  // include Mac OS X verions of headers
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#endif  // __APPLE__

#include "../common/common.h"
#include "../common/initShader.h"
#include "../common/geo_objs.h"
#include <stdio.h>

int num_vertices;
GLuint ctm_location;
mat4 identity_ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
mat4 ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint model_view_location;
mat4 model_view = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint projection_location;
mat4 projection = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
int isAnimating = 0;

float angle = 0.0;
float r_angle = 0.0;
float y_angle = 0.0;
float x_angle = 0.0;
float s_angle = 0.0;
vec4 eye = {0, 0, 5, 1};
vec4 at = {0, 0, 0, 1};
vec4 up = {0, 1, 0, 0};

int sphere_num_vertices, cube_num_vertices, num_vertices;
mat4 sphere_ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
mat4 cube_ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

GLuint use_texture_location;
int use_texture = 0;

void init(void)
{
    // Generate a sphere

    vec4 *sphere_positions = gen_sphere_vertices(&sphere_num_vertices, 10.0);
    vec4 *sphere_colors = gen_sphere_colors(sphere_positions, sphere_num_vertices, (vec4) {0, 0, 1, 1});
    vec4 *sphere_normals = gen_sphere_normals(sphere_positions, sphere_num_vertices);
    vec2 *sphere_tex_coords = gen_sphere_tex_coords(sphere_positions, sphere_num_vertices, (vec2) {0.5, 0.5}, (vec2) {0.75, 0.75});
    
    // Generate a cube

    vec4 *cube_positions = gen_cube_vertices(&cube_num_vertices);
    vec4 *cube_colors = gen_cube_colors((vec4) {1, 0, 0, 1}, (vec4) {0, 1, 0, 1}, (vec4) {0, 0, 1, 1},
					(vec4) {1, 1, 0, 1}, (vec4) {1, 0, 1, 1}, (vec4) {0, 1, 1, 1});
    vec4 *cube_normals = gen_cube_normals();
    vec2 *cube_tex_coords = gen_cube_tex_coords((vec2) {0.00, 0.00}, (vec2) {0.25, 0.25},
						(vec2) {0.25, 0.00}, (vec2) {0.50, 0.25},
						(vec2) {0.50, 0.00}, (vec2) {0.75, 0.25},
						(vec2) {0.75, 0.00}, (vec2) {1.00, 0.25},
						(vec2) {0.00, 0.25}, (vec2) {0.25, 0.50},
						(vec2) {0.25, 0.25}, (vec2) {0.50, 0.50});

    // Generate a floor
    
    // Start modifying colors here

    
    
    // End here

    num_vertices = sphere_num_vertices + 2 * cube_num_vertices;
    vec4 *positions = (vec4 *) malloc(sizeof(vec4) * num_vertices);
    vec4 *colors = (vec4 *) malloc(sizeof(vec4) * num_vertices);
    vec4 *normals = (vec4 *) malloc(sizeof(vec4) * num_vertices);
    vec2 *tex_coords = (vec2 *) malloc(sizeof(vec2) * num_vertices);

    int v_index = 0;
    
    for(int i = 0; i < sphere_num_vertices; i++) {
	positions[v_index] = sphere_positions[i];
	colors[v_index] = sphere_colors[i];
	normals[v_index] = sphere_normals[i];
	tex_coords[v_index] = sphere_tex_coords[i];
	v_index++;
    }

    for(int i = 0; i < cube_num_vertices; i++) {
	positions[v_index] = cube_positions[i];
	colors[v_index] = cube_colors[i];
	normals[v_index] = cube_normals[i];
	tex_coords[v_index] = cube_tex_coords[i];
	v_index++;
    }

    mat4 temp_tr = m4m4_multiplication(translate(0, -1, 0), scale(5, 0.1, 5));
    
    for(int i = 0; i < cube_num_vertices; i++) {
	positions[v_index] = m4v4_multiplication(temp_tr, cube_positions[i]);
	colors[v_index] = cube_colors[i];
	normals[v_index] = cube_normals[i];
	tex_coords[v_index] = cube_tex_coords[i];
	v_index++;
    }

    // modify colors[] according to lighting model

    // ambient
    
    vec4 *ambient_colors = (vec4 *) malloc(sizeof(vec4) * num_vertices);

    for(int i = 0; i < num_vertices; i++) {
        ambient_colors[i] = sv4_multiplication(0.3, colors[i]);
    }
    
    // diffuse

    vec4 light_position = {4, 4, 4, 1};

    vec4 *diffuse_colors = (vec4 *) malloc(sizeof(vec4) * num_vertices);
    
    for(int i = 0; i < num_vertices; i++) {
        vec4 l = v4_normalize(v4v4_subtraction(light_position, positions[i]));
        vec4 n = normals[i];
        float l_dot_n = v4v4_dot(l, n);
        if(l_dot_n < 0)
            l_dot_n = 0;
        diffuse_colors[i] = sv4_multiplication(l_dot_n, colors[i]);
    }

    // specular
    
    vec4 *specular_colors = (vec4 *) malloc(sizeof(vec4) * num_vertices);

    for(int i = 0; i < num_vertices; i++) {
        vec4 l = v4_normalize(v4v4_subtraction(light_position, positions[i]));
        vec4 v = v4_normalize(v4v4_subtraction(eye, positions[i]));
        vec4 h = v4_normalize(v4v4_addition(l, v));
        vec4 n = normals[i];

        float h_dot_n = v4v4_dot(h, n);
        if(h_dot_n < 0)
            h_dot_n = 0;

        specular_colors[i] = sv4_multiplication(pow(h_dot_n, 50), (vec4) {1, 1, 1, 1});
    }


    
    for(int i = 0; i < num_vertices; i++) {
        colors[i] = v4v4_addition(specular_colors[i], v4v4_addition(diffuse_colors[i], ambient_colors[i]));
    }
        


    
    // Texture coordinate of the square

    int tex_width = 64;
    int tex_height = 64;
    GLubyte my_texels[tex_width][tex_height][3];

    FILE *fp = fopen("textures01.raw", "r");
    if(fp == NULL) {
	printf("[textureTemplate] Successfully open a texture file.\n");
	exit(0);
    }
    fread(my_texels, tex_width * tex_height * 3, 1, fp);
    fclose(fp);

    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    GLuint mytex[1];
    glGenTextures(1, mytex);
    glBindTexture(GL_TEXTURE_2D, mytex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texels);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

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
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * num_vertices));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * 2 * num_vertices));

    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * 3 * num_vertices));

    ctm_location = glGetUniformLocation(program, "ctm");
    model_view_location = glGetUniformLocation(program, "model_view");
    projection_location = glGetUniformLocation(program, "projection");
    
    GLuint texture_location = glGetUniformLocation(program, "texture");
    glUniform1i(texture_location, 0);

    use_texture_location = glGetUniformLocation(program, "use_texture");
    glUniform1i(use_texture_location, use_texture);

    model_view = look_at_v4(eye, at, up);
    projection = frustum(-1, 1, -1, 1, -1, -100);

    cube_ctm = translate(2, 0, 0);
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *) &model_view);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *) &projection);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &sphere_ctm);
    glDrawArrays(GL_TRIANGLES, 0, sphere_num_vertices);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_ctm);
    glDrawArrays(GL_TRIANGLES, sphere_num_vertices, cube_num_vertices);

    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &identity_ctm);
    glDrawArrays(GL_TRIANGLES, sphere_num_vertices + cube_num_vertices, cube_num_vertices);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q') {
	#ifndef __APPLE__
	exit(0);
	#else
	glutLeaveMainLoop();
	#endif
    }
    else if(key == ' ') {
	isAnimating ^= 0x1;
    }
    else if(key == 'w') {
	x_angle -= 1.0;
	vec4 temp_eye = m4v4_multiplication(m4m4_multiplication(rotate_y(y_angle), rotate_x(x_angle)), eye);
	model_view = look_at_v4(temp_eye, at, up);
	glutPostRedisplay();
    }
    else if(key == 's') {
	x_angle += 1.0;
	vec4 temp_eye = m4v4_multiplication(m4m4_multiplication(rotate_y(y_angle), rotate_x(x_angle)), eye);
	model_view = look_at_v4(temp_eye, at, up); 
	glutPostRedisplay();
    }
    else if(key == 'a') {
	y_angle -= 1.0;
	vec4 temp_eye = m4v4_multiplication(m4m4_multiplication(rotate_y(y_angle), rotate_x(x_angle)), eye);
	model_view = look_at_v4(temp_eye, at, up);
	glutPostRedisplay();
    }
    else if(key == 'd') {
	y_angle += 1.0;
	vec4 temp_eye = m4v4_multiplication(m4m4_multiplication(rotate_y(y_angle), rotate_x(x_angle)), eye);
	model_view = look_at_v4(temp_eye, at, up);
	glutPostRedisplay();
    }
    else if(key == 't') {
	use_texture ^= 0x1;
	glUniform1i(use_texture_location, use_texture);
	glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
}

void motion(int x, int y) {
}

void idle(void) {
    if(isAnimating) {
	angle += 0.1;
	r_angle += 0.2;
	cube_ctm = m4m4_multiplication(rotate_y(angle), m4m4_multiplication(translate(2, 0, 0), rotate_x(r_angle)));
	s_angle -= 1;
	sphere_ctm = rotate_y(s_angle);
	//glutPostRedisplay();
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Texture Template");
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
