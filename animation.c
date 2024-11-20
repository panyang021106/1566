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
vec4 temp_eye = multiply_m4_vec4(multiply_m4_m4(rotating_y_m4(y_angle),
rotating_x_m4(x_angle)), eye);
model_view = look_at_v4(temp_eye, at, up);
glutPostRedisplay();
}
else if(key == 's') {
x_angle += 1.0;
vec4 temp_eye = multiply_m4_vec4(multiply_m4_m4(rotating_y_m4(y_angle),
rotating_x_m4(x_angle)), eye);
model_view = look_at_v4(temp_eye, at, up);
glutPostRedisplay();
}
else if(key == 'a') {
y_angle -= 1.0;
vec4 temp_eye = multiply_m4_vec4(multiply_m4_m4(rotating_y_m4(y_angle),
rotating_x_m4(x_angle)), eye);
model_view = look_at_v4(temp_eye, at, up);
glutPostRedisplay();
}
else if(key == 'd') {
y_angle += 1.0;
vec4 temp_eye = multiply_m4_vec4(multiply_m4_m4(rotating_y_m4(y_angle),
rotating_x_m4(x_angle)), eye);
model_view = look_at_v4(temp_eye, at, up);
glutPostRedisplay();
}
    model_view = look_at_v4(eye, at, up);
	glutPostRedisplay();
   
}

mat4 look_at_v4(vec4 eye, vec4 at, vec4 up) {
    // Compute forward vector (z-axis)
    vec4 forward = normalize_v4(sub_v4(at, eye));

    // Compute right vector (x-axis)
    vec4 right = normalize_v4(cross_product_v4(forward, up));

    // Compute up vector (y-axis)
    vec4 up_actual = cross_product_v4(right, forward);

    // Create rotation matrix
    mat4 rotation = {
        {right.x, up_actual.x, -forward.x, 0.0},
        {right.y, up_actual.y, -forward.y, 0.0},
        {right.z, up_actual.z, -forward.z, 0.0},
        {0.0,      0.0,        0.0,       1.0}
    };

    // Create translation matrix
    mat4 translation = {
        {1.0, 0.0, 0.0, -eye.x},
        {0.0, 1.0, 0.0, -eye.y},
        {0.0, 0.0, 1.0, -eye.z},
        {0.0, 0.0, 0.0, 1.0}
    };

    // Combine rotation and translation
    return multiply_m4_m4(rotation, translation);
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
    if (button ==4) { 
        eye = multiply_m4_vec4(scaling_m4(1.02, 1.02, 1.02),eye); 
    }
    else if (button == 3) { // Scroll down: zoom out
        eye = multiply_m4_vec4(scaling_m4(1/ 1.02, 1 /1.02, 1 / 1.02),eye); 
    }
    vec4 temp_eye = multiply_m4_vec4(multiply_m4_m4(rotating_y_m4(y_angle), rotating_x_m4(x_angle)), eye);
	model_view = look_at_v4(temp_eye, at, up);

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
    current_rotation_matrix = multiply_m4_m4(rotation_matrix, current_rotation_matrix);
    eye = multiply_m4_vec4(rotation_matrix, eye);
    model_view = look_at_v4(eye, at, up);
    last_touch = touch;
    touch = current_touch;
    glutPostRedisplay();

}

void idle()
{
if(isAnimating) {
x_angle += 0.1;
y_angle += 0.2;
identity_ctm = multiply_m4_m4(rotating_y_m4(x_angle),
multiply_m4_m4(translation_m4(2, 0, 0), rotating_x_m4(y_angle)));
z_angle -= 1;
//glutPostRedisplay();
}
glutPostRedisplay();
}
