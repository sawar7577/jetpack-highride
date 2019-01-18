#include "main.h"
#include "timer.h"
#include "ball.h"
#include <iostream>
#include <iomanip>      // std::setprecision
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1, ball2;
Player playa;
Floor level;
Magnet mag;
Fireline fr,fr2;

int flag;

glm::vec3 rotateAxes(glm::vec3 axis, float rotation){
    glm::mat4 rotationMat(1); // Creates a identity matrix
    rotationMat = glm::rotate(rotationMat, rotation, glm::vec3(0.0, 0.0, 1.0));
    glm::vec3 ret =  glm::vec3(rotationMat * glm::vec4(axis, 1.0));
    return ret;
}
bool check(glm::vec3 l, Rectangle a, Player b) {
   if(abs(glm::dot(glm::vec3(a.position.x-b.position.x, a.position.y-b.position.y, 0), l)) > (double)(a.width/2)*abs(glm::dot(rotateAxes(glm::vec3(1,0,0),a.rotation),l)) + (double)(a.height/2)*abs(glm::dot(rotateAxes(glm::vec3(0,1,0),a.rotation),l)) + (double)(b.width/2)*abs(glm::dot(rotateAxes(glm::vec3(1,0,0),b.rotation),l)) + (double)(b.height/2)*abs(glm::dot(rotateAxes(glm::vec3(0,1,0),b.rotation),l))){
        return true;
    }
    else { 
        return false;
    }
}
bool collision(Rectangle a, Player b) {
    if(check(rotateAxes(glm::vec3(1,0,0),a.rotation),a,b) 
    || check(rotateAxes(glm::vec3(0,1,0),a.rotation),a,b) 
    || check(rotateAxes(glm::vec3(1,0,0),b.rotation),a,b) 
    || check(rotateAxes(glm::vec3(0,1,0),b.rotation),a,b)){
        return false;
    } 
    else {
        return true;
    }
}


float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
// glm::vec3 eye ( 0, 0, 1 );
// glm::vec3 target (0, 0, 0);

void draw(GLFWwindow *window) {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    glm::vec3 eye (screen_center_x, 0, 2 );
    
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (screen_center_x, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ball1.draw(VP);
    ball2.draw(VP);
    playa.draw(VP);
    mag.draw(VP);
    // fr.r1.draw(VP);
    // fr.r2.draw(VP);
    fr.draw(VP);
    fr2.draw(VP);

    level.draw(VP, target.x);

}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    if (left) {
        // Do something
    }
}

void tick_elements(GLFWwindow *window) {
    if(flag == 0){
    ball1.tick();
    ball2.tick();
    // mag.tick(playa);

    playa.tick(window);
    }
    // camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball1       = Ball(2.0f, 0, COLOR_RED);
    ball2       = Ball(-2.0f, 0, COLOR_GREEN);
    ball2.speed = -1;
    playa       = Player(screen_center_x, screen_center_y, 0.75f, 1.15f, 2.0f, COLOR_BLACK);
    mag         = Magnet(2.0f, 2.0f, 1.0f, 1.0f, COLOR_BLACK);
    fr          = Fireline(4.0f,2.0f,2.0f,M_PI/3,COLOR_GREEN);
    fr2          = Fireline(1.0f,2.0f,2.0f,M_PI/6,COLOR_GREEN);    
    level       = Floor(-10.0f, -4.0f);

    // Create and compile our GLSL program from the shaders
    
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");;
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    flag = 0;
    srand(time(0));
    int width  = 1280;
    int height = 720;
    // flag = 1;
    window = initGLFW(width, height);

    initGL (window, width, height);
    /* Draw in loop */
    int loop = 1;
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        
     
        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
           
            draw(window);
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements(window);
            tick_input(window);
            // if(flag == 0 && collision(fr.r1, playa)){
            //     flag = 1;
            // }

            // if(flag == 0 && collision(fr.r2, playa)){
            //     flag = 1;
            // }
            // if(flag == 0 && collision(fr.r3, playa)){
            //     flag = 1;
            // }
            // if(flag == 0 && collision(Rectangle(mag.position.x,mag.position.y,mag.width,mag.height,mag.rotation,COLOR_RED), playa)){
            //     playa.momentum = glm::vec3(0);   
            // }
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    float apratio = 1280.0/720.0; 
    Matrices.projection = glm::ortho(left*apratio, right*apratio, bottom, top, 0.1f, 500.0f);
}

