#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
    public:
        Ball() {}
        Ball(float x, float y, color_t color);
        glm::vec3 position;
        float rotation;
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick();
        double speed;
    private:
        VAO *object;
};

class Player {
    public:
        float mass;
        glm::vec3 momentum;    
        Player() {}
        Player(float x, float y, float mass, color_t color);
        glm::vec3 position;
        float rotation;
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick(GLFWwindow *window);
    private:
        VAO *object;
};

class Rectangle {
    public:
        float rotation;
        Rectangle() {}
        Rectangle(float x, float y, float w, float h, float rot , color_t color);
        glm::vec3 position;
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick(GLFWwindow *window);
    private:
        VAO *object;
};

class Floor {
    public:
        std::vector <Rectangle> tiles;
        int map;
        Floor() {}
        Floor(float x, float y);
        glm::vec3 position;
        float rotation;
        void draw(glm::mat4 VP, float x);
        void set_position(float x, float y);
        void tick(GLFWwindow *window);
    private:
        VAO *object;
};

class Magnet {
    public:
        Magnet() {}
        Magnet(float x, float y, color_t color);
        glm::vec3 position;
        float rotation;
        float force;
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick(Player &player);
    private:
        VAO *object;
};

class Fireline {
    public:
        Rectangle r1;
        Rectangle r2;
        Rectangle r3;
        Fireline() {}
        Fireline(float x, float y, float length, float rot, color_t color);
        glm::vec3 position;
        float rotation;
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick();
    private:
        VAO *object;
};


#endif // BALL_H
