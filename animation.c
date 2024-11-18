#ifdef __APPLE__ // include Mac OS X verions of headers
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#endif // __APPLE__


// Animation states
typedef enum {
    NONE = 0,
    FLYING_UP,
    FLYING_DOWN,
    WALK_FORWARD,
    WALK_BACKWARD,
    TURN_LEFT,
    TURN_RIGHT,
    LOOK_UP,
    LOOK_DOWN
} AnimationState;

int isAnimating = 1;          // Animation active flag
int current_step = 0;         // Current animation step
int max_steps = 100;          // Total steps for the animation
AnimationState currentState = NONE; // Current animation type
float changing_vector[3] = {0.0, 0.0, 0.0}; // Animation movement vector
float starting_position[3] = {0.0, 0.0, 0.0}; // Starting position
float current_position[3] = {0.0, 0.0, 0.0}; // Current position
float camera_position[3] = {0.0, 0.0, 0.0};
float rotation_angle = 0.0;  // Rotation angle for TURN animations

void idle() {
    if (isAnimating) {
        current_step++; // Increment the animation step
        GLfloat alpha;

        if (currentState == NONE) {
            // No animation
        }
        else if (currentState == FLYING_UP) {
            if (current_step == max_steps) {
                isAnimating = 0;
                alpha = 1.0;
                current_position[1] = (alpha * changing_vector[1]) + starting_position[1];
                // Apply the final transformation
            } else {
                alpha = (float)current_step / max_steps;
                current_position[1] = (alpha * changing_vector[1]) + starting_position[1];
                // Apply intermediate transformation
            }
        }
        else if (currentState == FLYING_DOWN) {
            if (current_step == max_steps) {
                isAnimating = 0;
                alpha = 1.0;
                current_position[1] = (alpha * changing_vector[1]) + starting_position[1];
            } else {
                alpha = (float)current_step / max_steps;
                current_position[1] = (alpha * changing_vector[1]) + starting_position[1];
            }
        }
        else if (currentState == WALK_FORWARD) {
            if (current_step == max_steps) {
                isAnimating = 0;
                alpha = 1.0;
                current_position[2] = (alpha * changing_vector[2]) + starting_position[2];
            } else {
                alpha = (float)current_step / max_steps;
                current_position[2] = (alpha * changing_vector[2]) + starting_position[2];
            }
        }
        else if (currentState == WALK_BACKWARD) {
            if (current_step == max_steps) {
                isAnimating = 0;
                alpha = 1.0;
                current_position[2] = (alpha * changing_vector[2]) + starting_position[2];
            } else {
                alpha = (float)current_step / max_steps;
                current_position[2] = (alpha * changing_vector[2]) + starting_position[2];
            }
        }
        else if (currentState == TURN_LEFT) {
            if (current_step == max_steps) {
                isAnimating = 0;
                alpha = 1.0;
                rotation_angle = (alpha * changing_vector[0]) + starting_position[0];
            } else {
                alpha = (float)current_step / max_steps;
                rotation_angle = (alpha * changing_vector[0]) + starting_position[0];
            }
        }
        else if (currentState == TURN_RIGHT) {
            if (current_step == max_steps) {
                isAnimating = 0;
                alpha = 1.0;
                rotation_angle = (alpha * changing_vector[0]) + starting_position[0];
            } else {
                alpha = (float)current_step / max_steps;
                rotation_angle = (alpha * changing_vector[0]) + starting_position[0];
            }
        }
        else if (currentState == LOOK_UP) {
            if (current_step == max_steps) {
                isAnimating = 0;
                alpha = 1.0;
                camera_position[1] = (alpha * changing_vector[1]) + starting_position[1];
            } else {
                alpha = (float)current_step / max_steps;
                camera_position[1] = (alpha * changing_vector[1]) + starting_position[1];
            }
        }
        else if (currentState == LOOK_DOWN) {
            if (current_step == max_steps) {
                isAnimating = 0;
                alpha = 1.0;
                camera_position[1] = (alpha * changing_vector[1]) + starting_position[1];
            } else {
                alpha = (float)current_step / max_steps;
                camera_position[1] = (alpha * changing_vector[1]) + starting_position[1];
            }
        }

        glutPostRedisplay(); 
    }
}
