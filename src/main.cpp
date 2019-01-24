#include "main.h"
#include "timer.h"
#include "ball.h"
// #include "lists.h"
// #include "collision.h"

// #include "lists.h"
#include <iostream>
#include <iomanip>      // std::setprecision
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Player playa;
Floor level;
Magnet mag;
Numbers num;
CooldownBar cl;
Viserion vs;
int flag;


glm::vec3 rotateAxes(glm::vec3 axis, float rotation){
    glm::mat4 rotationMat(1); // Creates a identity matrix
    rotationMat = glm::rotate(rotationMat, rotation, glm::vec3(0.0, 0.0, 1.0));
    glm::vec3 ret =  glm::vec3(rotationMat * glm::vec4(axis, 1.0));
    return ret;
}

bool check(glm::vec3 l, Rectangle a, Rectangle b) {
   if(abs(glm::dot(glm::vec3(a.position.x-b.position.x, a.position.y-b.position.y, 0), l)) > (double)(a.width/2)*abs(glm::dot(rotateAxes(glm::vec3(1,0,0),a.rotation),l)) + (double)(a.height/2)*abs(glm::dot(rotateAxes(glm::vec3(0,1,0),a.rotation),l)) + (double)(b.width/2)*abs(glm::dot(rotateAxes(glm::vec3(1,0,0),b.rotation),l)) + (double)(b.height/2)*abs(glm::dot(rotateAxes(glm::vec3(0,1,0),b.rotation),l))){
        return true;
    }
    else { 
        return false;
    }
}
bool collision(Rectangle a, Rectangle b) {
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


template <class Type1, class Type2> bool collision_checker(const Type1 &a, const Type2 &b) {
    for(int i = 0 ; i < a.recs.size() ; ++i) {
        for(int j = 0 ; j < b.recs.size() ; ++j) {
            if(collision(a.recs[i] , b.recs[j])){
                return true;
            }
        }
    }
    return false;
}

template <typename Type> void draw_sprites(list <Type> &l, glm::mat4 VP) {
    typename list <Type>::iterator it;
    for(it = l.begin() ; it != l.end() ; ++it) {
        (*it).draw(VP);
    }
}

template <typename Type> void tick_sprites(list <Type> &l) {
    typename list <Type> :: iterator it;
    for(it = l.begin() ; it != l.end() ; ++it) {
        (*it).tick();
    }
}

void tick_magnet(list <Magnet> &l, Player &player) {
    list <Magnet> :: iterator it;
    for(it = l.begin() ; it != l.end() ; ++it) {
        (*it).tick(player);
    }
}

void magnet_response(list <Magnet> &l, Player &player) {
    list <Magnet> :: iterator it;
    for(it = l.begin() ; it != l.end() ; ++it) {
        if(collision_checker((*it), player)) {
            player.momentum = glm::vec3(0, 0, 0);
        }
    }
}

template <typename Type> void remove_balloons(list <Type> &l) {
    float top    = screen_center_y + vertical_float / screen_zoom;
    float bottom = screen_center_y - vertical_float / screen_zoom;
    float left   = screen_center_x - horizontal_float / screen_zoom;
    float right  = screen_center_x + horizontal_float / screen_zoom;
    // std::cout << l.size() << std::endl;
    typename list <Type> :: iterator it = l.begin();
    while(it != l.end()) {
        if((*it).position.y < bottom) {
            l.erase(it++);
        }
        else {
            it++;
        }
    }
}

void remove_flares(list <Jetflare> &l) {
    list <Jetflare> :: iterator it = l.begin();
    while(it != l.end()) {
        if( (std::clock() - (*it).start)/(double)CLOCKS_PER_SEC > 0.5f ) {
            l.erase(it++);
        }
        else {
            it++;
        }
    }
}

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0, vertical_float = 4, horizontal_float  = 4;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */

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
    draw_sprites(ball_list, VP);
    draw_sprites(fireline_list, VP);    
    draw_sprites(firebeam_list, VP);
    draw_sprites(boomerang_list, VP);   
    draw_sprites(powerup_list, VP);
    draw_sprites(waterballoon_list, VP);
    draw_sprites(magnet_list, VP);
    draw_sprites(jetflare_list, VP);
    num.draw(VP);
    playa.draw(VP);
    cl.draw(VP);
    vs.draw(VP);
    // b.draw(VP);

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
    tick_sprites(ball_list);
    tick_sprites(firebeam_list);    
    tick_sprites(boomerang_list);
    tick_sprites(powerup_list);
    tick_sprites(waterballoon_list);
    tick_sprites(jetflare_list);
    tick_magnet(magnet_list,playa);
    magnet_response(magnet_list,playa);
    remove_balloons(waterballoon_list);
    remove_flares(jetflare_list);
    playa.tick(window);
    cl.tick((std::clock() - playa.cooldown)/(double)CLOCKS_PER_SEC);
    num.tick();
    vs.tick();
    // camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    // ball1       = Ball(2.0f, 0, COLOR_RED);
    // ball2       = Ball(-2.0f, 0, COLOR_GREEN);
    playa       = Player(screen_center_x, screen_center_y, 0.75f, 1.15f, 2.0f, COLOR_BLACK);
    level       = Floor(-10.0f, -4.0f);
    num         = Numbers(-2.0f, 2.0f, 0.5f, 1.0f, "1111110",COLOR_BLACK);
    cl          = CooldownBar(-3.0f, -1.0f, 10.0f, 1.0f, COLOR_BLACK);
    vs          = Viserion(-10.0f, -1.0f, 1.0f, 1.0f, COLOR_BLACK);
    magnet_list.push_back(Magnet(2.0f, 2.0f, 1.0f, 1.0f, COLOR_BLACK));
    boomerang_list.push_back(Boomerang(6.0f,-1.0f,0.0f,0.0f,1.0f,1.0f,COLOR_BLACK));
    firebeam_list.push_back(Firebeam(6.0f,2.0f,2.0f,0,COLOR_GREEN));
    fireline_list.push_back(Fireline(4.0f,2.0f,2.0f,M_PI/3,COLOR_GREEN));
    fireline_list.push_back(Fireline(10.0f,2.0f,2.0f,M_PI/6,COLOR_GREEN));
    powerup_list.push_back(Powerup(2.0f, 2.0f, 1.0f, 1.0f, 1.0f, COLOR_FIRERED));
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
    float top    = screen_center_y + vertical_float / screen_zoom;
    float bottom = screen_center_y - vertical_float / screen_zoom;
    float left   = screen_center_x - horizontal_float / screen_zoom;
    float right  = screen_center_x + horizontal_float / screen_zoom;
    float apratio = 1280.0/720.0; 
    Matrices.projection = glm::ortho(left*apratio, right*apratio, bottom, top, 0.1f, 500.0f);
}

