void keyboard(unsigned char key, int mousex, int mousey)
{
    if (key == 'q')
    {
#ifdef __APPLE__
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
        eye = sub_v4((vec4){-2.0, 1.0, 2.0, 1.0}, eye);
        at = sub_v4((vec4){0.0, 0.0, 0.0, 1.0},at);
        up = sub_v4((vec4){0.0, 1.0, 0.0, 0.0}, up);
        model_view = look_at(eye, at, up);
        glutPostRedisplay();
    }
    else if (key == 'n')
    {
        isAnimating = 0;
    
    vec4 direction = sub_v4(at, eye);
    mat4 rotation = rotating_y_m4(-5.0); 
    vec4 rotated_direction = multiply_m4_vec4(rotation, direction);
    at = add_v4(eye, rotated_direction);
    model_view = look_at(eye, at, up);
    glutPostRedisplay();
    }
    else if (key == 'm')
    {
        isAnimating = 0;
    vec4 direction = sub_v4(at, eye);
    mat4 rotation = rotating_y_m4(5.0); 
    vec4 rotated_direction = multiply_m4_vec4(rotation, direction);
    at = add_v4(eye, rotated_direction);
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
