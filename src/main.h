#ifndef MAIN_H
#define MAIN_H

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <fstream>
#include <vector>
#include <list>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>
// #include "ball.h"


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct color_t {
    int r;
    int g;
    int b;
};


// nonedit.cpp
unsigned long xorshf96(void);
GLFWwindow *initGLFW(int width, int height);
GLuint     LoadShaders(const char *vertex_file_path, const char *fragment_file_path);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat *color_buffer_data, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const color_t color, GLenum fill_mode = GL_FILL);
void       draw3DObject(struct VAO *vao);

// input.cpp
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods);
void keyboardChar(GLFWwindow *window, unsigned int key);
void mouseButton(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// other_handlers.cpp
void error_callback(int error, const char *description);
void quit(GLFWwindow *window);
void reshapeWindow(GLFWwindow *window, int width, int height);

// Types
struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int    NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    GLuint    MatrixID;
};

extern GLMatrices Matrices;

// ---- Logic ----

enum direction_t { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT };

struct bounding_box_t {
    float x;
    float y;
    float width;
    float height;
    float rotation;
};

bool detect_collision(bounding_box_t a, bounding_box_t b);

extern float screen_zoom, screen_center_x, screen_center_y, vertical_float, horizontal_float;
void reset_screen();

// ---- Colors ----
extern const color_t COLOR_RED;
extern const color_t COLOR_FIRERED;
extern const color_t COLOR_FIREYELLOW;
extern const color_t COLOR_GREEN;
extern const color_t COLOR_BLACK;
extern const color_t COLOR_BACKGROUND;
extern const color_t COLOR_SILVER;
extern const color_t COLOR_OXFORDBLUE;
extern const color_t COLOR_SHAMROCKGREEN;
extern const color_t COLOR_CHOCOLATE;
extern const color_t COLOR_SKOBELOFF;
extern const color_t COLOR_TOPAZ;
extern const color_t COLOR_HARVESTGOLD;
extern const color_t COLOR_FERRARIRED;
extern const color_t COLOR_CHROMEYELLOW;
extern const color_t COLOR_FOLLY;




#endif
