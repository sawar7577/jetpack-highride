#include "main.h"
#include <list>

using namespace std;

#ifndef BALL_H
#define BALL_H


class Rectangle {
    public:
        float rotation;
        float width;
        float height;
        clock_t start;
        Rectangle() {}
        Rectangle(float x, float y, float w, float h, float rot , color_t color);
        glm::vec3 position;
        glm::vec3 momentum;
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick();
    private:
        VAO *object;
};

class Sprite {
    public:
        std::vector <Rectangle> recs;
        std::vector <glm::vec3> relpos;
        std::vector <Rectangle> broad_phase;
        float width;
        float height;
        float rotation;
        color_t color;
        glm::vec3 position;
        Sprite() {}
        Sprite(float x, float y, float width, float height, float rotation, color_t color);
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick();
        void destroy();
};


class Player : public Sprite {
    public:
        int score;
        float mass;
        float speedy;
        bool invincibility;
        int lives;
        clock_t inv;
        clock_t cooldown;
        glm::vec3 momentum;
        Player() {} 
        Player(float x, float y, float width, float height, float mass, color_t color);
        void tick(GLFWwindow *window);
};

class Fireline : public Sprite {
    public:
        Fireline() {}
        Fireline(float x, float y, float length, float rot, color_t color);
        void action(Player &player);
};


class Boomerang : public Sprite {
    public:
        float center_x;
        float center_y;
        Boomerang() {}
        Boomerang(float x, float y, float center_x, float center_y, float width, float height, color_t color);
        void tick();
        void action(Player &player);
};


class Firebeam : public Sprite {
    public:
        float speed;
        Firebeam() {}
        Firebeam(float x, float y, float length, float rot, color_t color);
        void tick();
        void action(Player &player);
};

class Ball : public Sprite {
    public:
        int points;
        Ball() {}
        Ball(float x, float y, color_t color);
        void tick();
        void action(Player &player);
};


class Firebeamconfusion : public Sprite {
    public:
        clock_t start;
        Firebeamconfusion () {}
        Firebeamconfusion(float x, float y, float width, float height, color_t color);
        void tick(Player &player);
        void action(Player &player);
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
        Magnet(float x, float y, float width = 1.0f, float height = 1.0f, color_t color = COLOR_BLACK);
        float force;
        void tick(Player &player);
        void action(Player &player);
};

class Viserion : public Sprite {
    public:
        glm::vec3 momentum;
        float mass;
        Viserion() {}
        Viserion(float x, float y, float width = 2.0f, float height = 2.0f, color_t color = COLOR_BLACK);
        void tick();
        void action(Player &player);
};

class Iceball : public Sprite {
    public:
        float mass;
        glm::vec3 momentum;
        Iceball() {}
        Iceball(float x, float y, float width, float height, float mass, color_t color);
        void tick();
        void action(Player &player);
};


class Powerup : public Sprite {
    public:
        float mass;
        glm::vec3 momentum;
        Powerup() {}
        Powerup(float x, float y, float width, float height, float mass, color_t color);
        void tick();
        void action(Player &player);
};

class Sword : public Powerup {
    public:
        Sword() {}
        Sword(float x, float y, float width = 1.0f, float height = 1.0f, float mass = 1.0f, color_t color = COLOR_RED);
        void action(Player &player);
};

class Heart : public Powerup {
    public:
        Heart() {}
        Heart(float x, float y, float width = 1.0f, float height = 1.0f, float mass = 1.0f, color_t color = COLOR_RED);
        void action(Player &player);
};

class Bolt : public Powerup {
    public:
        Bolt() {}
        Bolt(float x, float y, float width = 1.0f, float height = 1.0f, float mass = 1.0f, color_t color = COLOR_RED);
        void action(Player &player);
};

class Jetflare : public Sprite {
    public: 
        clock_t start;
        float mass;
        glm::vec3 momentum;
        Jetflare() {}
        Jetflare(float x, float y, float width, float height, float mass, color_t color);
        void tick();
};

class Steam : public Sprite {
    public:
        clock_t start;
        float mass;
        glm::vec3 momentum;
        Steam() {}
        Steam(float x, float y, float width, float height, float mass, color_t color);
        void tick();
};

class WaterBalloon : public Sprite {
    public:
        float mass;
        glm::vec3 momentum;
        WaterBalloon() {}
        WaterBalloon(float x, float y, float width, float height, float mass, color_t color);
        void tick();
};

class Numbers : public Sprite {
    public:
        Numbers() {}
        Numbers(float x, float y, float width, float height, string hash, color_t color);
        void tick();
};

class CooldownBar : public Sprite {
    public:
        CooldownBar() {}
        CooldownBar(float x, float y, float width, float height, color_t color);
        void tick(float tme);
};

class Semicircle {
    public:
        float radius;
        glm::vec3 position;
        Semicircle() {}
        Semicircle(float x, float y, float radius, color_t color);
        void draw(glm::mat4 VP);
    private:
        VAO *object;
};

class Ring : public Sprite {
    public:
        float radius;
        float thick;
        bool activated;
        vector <Semicircle> semis;
        Ring() {}
        Ring(float x, float y, float radius = 4.0f, float thick = 0.5f, color_t color = COLOR_RED);
        void tick(Player &player, GLFWwindow *window);
        void draw(glm::mat4 VP);
};


extern list <Ball> ball_list;
extern list <Firebeam> firebeam_list;
extern list <Fireline> fireline_list;
extern list <Boomerang> boomerang_list;
extern list <Powerup> powerup_list;
extern list <WaterBalloon> waterballoon_list;
extern list <Magnet> magnet_list;
extern list <Jetflare> jetflare_list;
extern list <Steam> steam_list;
extern list <Iceball> iceball_list;
extern list <Ring> ring_list;
extern list <Viserion> viserion_list;
extern list <Sword> sword_list;
extern list <Heart> heart_list;
extern list <Bolt> bolt_list;
extern list <Firebeamconfusion> firebeamconfusion_list;
extern list <Rectangle> destroyed_list;



#endif // BALL_H
