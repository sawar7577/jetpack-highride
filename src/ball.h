#include "main.h"

#ifndef BALL_H
#define BALL_H

class Rectangle {
    public:
        float rotation;
        float width;
        float height;
        Rectangle() {}
        Rectangle(float x, float y, float w, float h, float rot , color_t color);
        glm::vec3 position;
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick(GLFWwindow *window);
    private:
        VAO *object;
};

class Sprite {
    public:
        std::vector <Rectangle> recs;
        std::vector <glm::vec3> relpos;
        float width;
        float height;
        float rotation;
        glm::vec3 position;
        Sprite() {}
        Sprite(float x, float y, float width, float height, float rotation, color_t color);
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick();
};

class Fireline : public Sprite {
    public:
        Fireline() {}
        Fireline(float x, float y, float length, float rot, color_t color);
};

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


class Player : public Sprite {
    public:
        float mass;
        glm::vec3 momentum;    
        Player() {}
        Player(float x, float y, float width, float height, float mass, color_t color);
        void tick(GLFWwindow *window);
};

class Floor : public Sprite {
    public:
        int map;
        Floor() {}
        Floor(float x, float y);
        void draw(glm::mat4 VP, float x);
        void tick(GLFWwindow *window);
};

class Magnet : public Sprite {
    public:        
        Magnet() {}
        Magnet(float x, float y, float width, float height, color_t color);
        float force;
        void tick(Player &player);
};




#endif // BALL_H
