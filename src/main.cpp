#include "main.h"
#include "timer.h"
#include "ball.h"

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

// int flag;

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
    glm::vec3 eye (screen_center_x, 0, 1 );
    
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (screen_center_x, 0, 0);
    // if(glfwGetKey(window, GLFW_KEY_RIGHT)){
    //     // target.x += 0.2f;
    //     // target.x = playa.position.x;
    //     // screen_center_x += 0.2f;
    //     eye.x = target.x;
    // }
    // if(glfwGetKey(window, GLFW_KEY_LEFT)){
    //     target.x -= 0.2f;
    //     // target.x = playa.position.x;

        // screen_center_x -= 0.2f;
    //     eye.x = target.x;
    // }
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
    ball1.tick();
    ball2.tick();
    mag.tick(playa);

    playa.tick(window);
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
    playa       = Player(screen_center_x, screen_center_y, 2.0f, COLOR_BLACK);
    level       = Floor(-10.0f, -4.0f);
    mag         = Magnet(2.0f, 2.0f, COLOR_BLACK);
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
