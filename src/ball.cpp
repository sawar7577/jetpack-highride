#include "ball.h"
#include "main.h"

list <Ball> ball_list;
list <Firebeam> firebeam_list;
list <Fireline> fireline_list;
list <Boomerang> boomerang_list; 
list <Powerup> powerup_list;
list <WaterBalloon> waterballoon_list;
list <Magnet> magnet_list;
list <Jetflare> jetflare_list;
list <Steam> steam_list;
list <Iceball> iceball_list;
list <Ring> ring_list;
list <Viserion> viserion_list;
list <Sword> sword_list;
list <Heart> heart_list;
list <Bolt> bolt_list;
list <Firebeamconfusion> firebeamconfusion_list;
list <Rectangle> destroyed_list;


void add_coin(list <Ball> &l) {
    for(int i = 0 ; i < 1 ; ++i) {
        int random = rand();
        random = -4000 + random % (7999);
        if(random%7 == 0){
            l.push_back(Ball(screen_center_x + 2*horizontal_float, (float)(random)/1000.0f ,COLOR_FIREYELLOW));
        }
    }
}

void add_jetflares(list <Jetflare> &l, float x, float y) {
	unsigned long ret = xorshf96();
	if(ret%4 == 0)
		l.push_back(Jetflare(x+0.1f, y, 0.075f, 0.2f, 10.0f, COLOR_FIRERED));
	else if(ret%4 == 1)
		l.push_back(Jetflare(x-0.1f, y, 0.075f, 0.2f, 10.0f, COLOR_FIREYELLOW));
	else if(ret%4 == 2)
		l.push_back(Jetflare(x+0.05f, y, 0.075f, 0.2f, 10.0f, COLOR_FIRERED));
	else if(ret%4 == 3)
		l.push_back(Jetflare(x-0.05f, y, 0.075f, 0.2f, 10.0f, COLOR_FIREYELLOW));
}

void add_iceballs(list <Iceball> &l, float x, float y) {
	unsigned long ret = xorshf96();
	if(ret%100 == 0) {
		l.push_back(Iceball(x,y, 1.0f, 1.0f, 1.0f, COLOR_BLACK));
	}
}

Sprite::Sprite(float x, float y, float width, float height, float rotation, color_t color) {
	this->position = glm::vec3(x, y, 0);
	this->width = width;
	this->height = height;
	this->rotation = rotation;
	this->color = color;
	this->broad_phase.push_back(Rectangle(x, y, width, height, rotation, color));
}

void Sprite::draw(glm::mat4 VP) {
	for(int i = 0 ; i < this->recs.size() ; ++i) {
		this->recs[i].draw(VP);
	}
}

void Sprite::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Sprite::destroy() {
	for(int i = 0 ; i < this->recs.size() ; ++i) {
		unsigned long ret = xorshf96();
		this->recs[i].start = std::clock();
		this->recs[i].momentum = glm::vec3(sin ( M_PI/(64*(1+ret%7) )) ,-1.4f*sin ( M_PI/(64*(1+ret%7) )),0);
		destroyed_list.push_back(this->recs[i]);
	}
}

void Sprite::tick() {
	for(int i = 0 ; i < this->recs.size() ; ++i) {
		this->recs[i].position = this->position + this->relpos[i];
	}
	for(int i = 0 ; i < this->broad_phase.size() ; ++i) {
		this->broad_phase[i].position = this->position;
		this->broad_phase[i].rotation = this->rotation;
	}
}

Ball::Ball(float x, float y, color_t color) : Sprite(x, y , 0.25, 0.25, M_PI/4, COLOR_FIREYELLOW){
	unsigned long ret = xorshf96();
	color_t c;
	if(ret % 2 == 0){
		this->points = 2;
		c = COLOR_FIREYELLOW;
	}
	else {
		this->points = 1;
		c = COLOR_FIRERED;
	}

	this->recs.push_back(Rectangle(x, y, 0.25f, 0.25f, M_PI/4, c));
}

void Ball::action(Player &player) {
	player.score += this->points;
}

Player::Player(float x, float y, float width, float height, float mass, color_t color) : Sprite(x,y,width,height,0,color) {
	this->mass = mass;
	this->score = 0;
	this->speedy = 0.0036f;
	this->momentum = glm::vec3(0, 0, 0);
	this->cooldown = std::clock();
	this->inv = -5.0f;
	this->invincibility = false;
	this->inring = false;
	this->lives = 15;
	this->recs.push_back(Rectangle(x+width/2-width/3,y-height/10,2*width/3,height/2,0,COLOR_SHAMROCKGREEN));
	this->relpos.push_back(glm::vec3(width/2-width/3,-height/10,0));
	this->recs.push_back(Rectangle(x-width/2+width/6,y,width/3,height/3,0,COLOR_OXFORDBLUE));
	this->relpos.push_back(glm::vec3(-width/2+width/6,0,0));	
	this->recs.push_back(Rectangle(x-width/2+width/6,y+height/8,width/5,2*height/8,0,COLOR_OXFORDBLUE));
	this->relpos.push_back(glm::vec3(-width/2+width/6,height/8,0));	
	this->recs.push_back(Rectangle(x+width/2-width/3,y-height/10+height/4+height/40,width/8,height/20,0,COLOR_CHOCOLATE));
	this->relpos.push_back(glm::vec3(width/2-width/3,-height/10+height/4+height/40,0));	
	this->recs.push_back(Rectangle(x+width/2-width/3,y-height/10+height/4+height/20+13*height/80,2*width/3,13*height/40,0,COLOR_TOPAZ));
	this->relpos.push_back(glm::vec3(width/2-width/3,-height/10+height/4+height/20+13*height/80,0));
	this->recs.push_back(Rectangle(x+width/2-width/3,y-height/10+height/4+height/20+13*height/80,2*width/3,13*height/80,0,COLOR_BLACK));
	this->relpos.push_back(glm::vec3(width/2-width/3,-height/10+height/4+3*height/20+13*height/80,0));
	this->recs.push_back(Rectangle(x+width/2-width/3+width/5,y-height/4-height/8,width/5,height/4,0,COLOR_SKOBELOFF));
	this->relpos.push_back(glm::vec3(width/2-width/3+width/5,-height/4-height/8,0));	
	this->recs.push_back(Rectangle(x+width/2-width/3-width/5,y-height/4-height/8,width/5,height/4,0,COLOR_SKOBELOFF));
	this->relpos.push_back(glm::vec3(width/2-width/3-width/5,-height/4-height/8,0));
	
}


void Player::tick(GLFWwindow *window) {

    float top    = screen_center_y + vertical_float / 1;
    float bottom = screen_center_y - 9*vertical_float / (10);

    this->momentum.x += (this->momentum.x*(-0.01f));
    	if(glfwGetKey(window,GLFW_KEY_LEFT)){
        this->momentum.x -= this->speedy;
    }
    if(glfwGetKey(window,GLFW_KEY_RIGHT)){
        this->momentum.x += this->speedy;
		add_coin(ball_list);
    }
    if(glfwGetKey(window,GLFW_KEY_SPACE) || glfwGetKey(window,GLFW_KEY_UP)){
		add_jetflares(jetflare_list, this->position.x - this->width/2, this->position.y);
        this->momentum.y += this->speedy;
    }
    if(glfwGetKey(window,GLFW_KEY_DOWN)){
        this->momentum.y -= this->speedy;
    }
	if(glfwGetKey(window,GLFW_KEY_B)){
		clock_t now = std::clock();
		if( (now-this->cooldown)/(double)CLOCKS_PER_SEC > 1.5f) {
			waterballoon_list.push_back(WaterBalloon(this->position.x, this->position.y, 0.75f, 0.75f, 1.0f, COLOR_GREEN));
			this->cooldown = now;
		}
	}
    this->momentum.y -= 0.00125f;
    this->position += this->momentum / this->mass;
	if(this->position.y - this->height/2 <= bottom){
		this->momentum.y = 0.0f;
		this->position.y = bottom+this->height/2;
	}
    if(this->position.y + this->height/2 >= top){
		this->momentum.y = 0.0f;
		this->position.y = top-this->height/2;
	}
    
	if( (std::clock() - this->inv)/(float)CLOCKS_PER_SEC > 5.f){
		this->invincibility = false;
	}
	screen_center_x = this->position.x;
	Sprite::tick();
}

Rectangle::Rectangle(float x, float y, float w, float h, float rot, color_t color) {
	this->position = glm::vec3(x,y,0);
    this->rotation = rot;
	this->width = w;
	this->height = h;
	this->momentum = glm::vec3(0,0,0);

	GLfloat vertex_buffer_data [] = {
		0.0f - this->width/2.0f, 0.0f + this->height/2.0f, 0.0f,
		0.0f + this->width/2.0f, 0.0f + this->height/2.0f, 0.0f,
		0.0f - this->width/2.0f, 0.0f - this->height/2.0f, 0.0f,
		0.0f + this->width/2.0f, 0.0f - this->height/2.0f, 0.0f,
		0.0f - this->width/2.0f, 0.0f - this->height/2.0f, 0.0f,
		0.0f + this->width/2.0f, 0.0f + this->height/2.0f, 0.0f,
	};
	this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
}

void Rectangle::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation), glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Rectangle::tick() {
	this->position += this->momentum;
}

Floor::Floor(float x, float y):Sprite(x,y,0,0,0,COLOR_BLACK){
	this->map = 0;
	Rectangle t;
	for(int i = 0 ; i < 40 ; ++i){
		if(i%2 == 0) {
			t = Rectangle(screen_center_x, screen_center_y - 19*vertical_float/20, vertical_float/5, vertical_float/10, 0, COLOR_RED);
		}
		else {
			t = Rectangle(screen_center_x, screen_center_y - 19*vertical_float/20, vertical_float/5, vertical_float/10, 0, COLOR_GREEN);
		}
		t.position.x += i*vertical_float/5;
		this->recs.push_back(t);
	}
}

void Floor::draw(glm::mat4 VP, float x) {
	if(this->recs[this->map].position.x - x < -14.0f){
		this->recs[this->map].position.x = this->recs[ (this->map+39)%40 ].position.x+vertical_float/5;
		this->map++; 
	}
	else{
		this->recs[(this->map+39)%40].position.x = this->recs[ this->map ].position.x-vertical_float/5;	
		this->map--;
	}
	this->map = (this->map+40)%40;
	Sprite::draw(VP);
}

Magnet::Magnet(float x, float y, float width, float height, color_t color):Sprite(x, y, width, height, 0, color){
	this->force = 0.040f;
	
	this->recs.push_back(Rectangle(x+width/2-width/10,y+height/2-5*height/10,width/5,height/5,0,COLOR_BLACK));
	this->recs.push_back(Rectangle(x+width/2-width/10,y+height/2-3*height/10,width/5,height/5,0,COLOR_BLACK));
	this->recs.push_back(Rectangle(x+width/2-width/10,y+height/2-7*height/10,width/5,height/5,0,COLOR_BLACK));
	this->recs.push_back(Rectangle(x+width/2-width/10,y+height/2-9*height/10,width/5,height/5,0,COLOR_RED));

	this->recs.push_back(Rectangle(x-width/2+width/10,y+height/2-3*height/10,width/5,height/5,0,COLOR_BLACK));
	this->recs.push_back(Rectangle(x-width/2+width/10,y+height/2-5*height/10,width/5,height/5,0,COLOR_BLACK));
	this->recs.push_back(Rectangle(x-width/2+width/10,y+height/2-7*height/10,width/5,height/5,0,COLOR_BLACK));
	this->recs.push_back(Rectangle(x-width/2+width/10,y+height/2-9*height/10,width/5,height/5,0,COLOR_RED));
	
	this->recs.push_back(Rectangle(x+width/2- 3*width/10,y+height/2-height/10,width/5,height/5,0,COLOR_BLACK));
	this->recs.push_back(Rectangle(x+width/2- 5*width/10,y+height/2-height/10,width/5,height/5,0,COLOR_BLACK));
	this->recs.push_back(Rectangle(x+width/2- 7*width/10,y+height/2-height/10,width/5,height/5,0,COLOR_BLACK));

}

void Magnet::tick(Player &player) {
	glm::vec3 dist = this->position - player.position;
	if(dist.x*dist.x + dist.y*dist.y >= 2.0f){
	float ef = (this->force/player.mass)/(dist.x*dist.x+dist.y*dist.y);
	glm::vec3 ch(ef*dist.x/sqrt(dist.x*dist.x+dist.y*dist.y),ef*dist.y/sqrt((dist.x*dist.x+dist.y*dist.y)),0);
	player.momentum.x += ch.x;
	player.momentum.y += ch.y;
	}
}

void Magnet::action(Player &player) {
	// if(!player.inring)
		player.momentum = glm::vec3(0,0,0);
}

Fireline::Fireline(float x, float y, float length, float rot, color_t color): Sprite(x, y, length + 0.7f, 0.55f, rot, color){
	this->position = glm::vec3(x, y, 0);
	this->recs.push_back(Rectangle(x,y,length,0.25f,rot,COLOR_CHROMEYELLOW));
	this->relpos.push_back(glm::vec3(0,0,0));
	this->recs.push_back(Rectangle((x)+cos(rot)*length/2.0f,y+(sin(rot))*length/2.0f,0.5f,0.5f,rot+M_PI/4,COLOR_FERRARIRED));
	this->relpos.push_back(glm::vec3(cos(rot)*length/2.0f,(sin(rot))*length/2.0f,0));	
	this->recs.push_back(Rectangle((x)+cos(rot+M_PI)*length/2.0f,y+(sin(rot+M_PI))*length/2.0f,0.5f,0.5f,rot+M_PI/4,COLOR_FERRARIRED));	
	this->relpos.push_back(glm::vec3(cos(rot+M_PI)*length/2.0f,(sin(rot+M_PI))*length/2.0f,0));	
}

void Fireline::action(Player &player) {
	if(!player.inring && !player.invincibility)
		player.lives--;
}

Firebeam::Firebeam(float x, float y, float length, float rot, color_t color) : Sprite(x, y, length + 0.7f, 0.55f, rot, color) {
	this->position = glm::vec3(x, y, 0);
	this->speed = 0.015f;
	this->recs.push_back(Rectangle(x,y,length,0.25f,0,COLOR_CHROMEYELLOW));
	this->relpos.push_back(glm::vec3(0,0,0));
	this->recs.push_back(Rectangle((x)+cos(rot)*length/2.0f,y+(sin(rot))*length/2.0f,0.5f,0.5f,0+M_PI/4,COLOR_FERRARIRED));
	this->relpos.push_back(glm::vec3(-length/2.0f,0,0));		
	this->recs.push_back(Rectangle((x)+cos(rot+M_PI)*length/2.0f,y+(sin(rot+M_PI))*length/2.0f,0.5f,0.5f,0+M_PI/4,COLOR_FERRARIRED));	
	this->relpos.push_back(glm::vec3(length/2.0f,0.0f,0));	

}

void Firebeam::tick() {
	float top    = screen_center_y + vertical_float / 1;
    float bottom = screen_center_y - vertical_float / 1;
    float left   = screen_center_x - horizontal_float / 1;
    float right  = screen_center_x + horizontal_float / 1;

	this->position.y += this->speed;

	if(this->position.y > top ) {
		this->position.y = top;
		this->speed *= -1;
	}
	if( this->position.y < bottom) {
		this->position.y = bottom;
		this->speed *= -1;
	}
	Sprite::tick();
}

void Firebeam::action(Player &player) {
	if(!player.inring && !player.invincibility)
		player.lives--;	
}

Boomerang::Boomerang(float center_x, float center_y, float width, float height, color_t color) : Sprite(0, 0, width, height, 0, color) {
	this->center_x = center_x;
	this->center_y = center_y;
	this->start = std::clock();
	this->recs.push_back(Rectangle(0, 0, width/4, height/4, M_PI/4, color));
	this->relpos.push_back(glm::vec3(width/2 - sqrt(width*width+height*height)/4*2, height/2 - sqrt(width*width+height*height)/4*2, 0));
	this->recs.push_back(Rectangle(0,0, width/4, height/4, M_PI/4, color));
	this->relpos.push_back(glm::vec3(width/2 - sqrt(width*width+height*height)/4, height/2 - sqrt(width*width+height*height)/4*2, 0));	
	this->recs.push_back(Rectangle(0,0, width/4, height/4, M_PI/4, color));
	this->relpos.push_back(glm::vec3(width/2 - sqrt(width*width+height*height)/4*2, height/2 - sqrt(width*width+height*height)/4, 0));
}

glm::vec3 rotateaxes(glm::vec3 axis, float rotation){
    glm::mat4 rotationMat(1); // Creates a identity matrix
    rotationMat = glm::rotate(rotationMat, rotation, glm::vec3(0.0, 0.0, 1.0));
    glm::vec3 ret =  glm::vec3(rotationMat * glm::vec4(axis, 1.0));
    return ret;
}

void Boomerang::tick() {
	this->rotation += 0.05f;
	this->position.x = 5.0f*cos(this->rotation) + this->center_x;
	this->position.y = 3.25f*sin(this->rotation) + this->center_y;
	for(int i = 0 ; i < this->recs.size() ; ++i) {
		this->recs[i].rotation += 0.25f;
		this->relpos[i] = rotateaxes(this->relpos[i],0.25f);
	}
	Sprite::tick();
}

void Boomerang::action(Player &player) {
	if(!player.inring && !player.invincibility)
		player.lives--;
}

Powerup::Powerup(float x, float y, float width, float height, float mass, color_t color) : Sprite(x, y, width, height, 0, color) {
	this->mass = mass;
	this->momentum = glm::vec3(-0.01f,0,0);
}

void Powerup::tick() {
	float top    = screen_center_y + vertical_float / 1;
    float bottom = screen_center_y - 9*vertical_float / (1*10);
	
	this->momentum.y -= 0.00125f;
    this->position += this->momentum / this->mass;
	if(this->position.y - this->height/2 < bottom){
		this->momentum.y *= (-1);
		this->position.y = bottom + this->height/2;
	}
    if(this->position.y + this->height/2 > top){
		this->momentum.y *= (-1);
		this->position.y = top - this->height/2;
	}
	Sprite::tick();

}

Sword::Sword(float x, float y, float width, float height, float mass, color_t color) : Powerup(x, y, width, height, mass, color) {

	this->recs.push_back(Rectangle(0, 0, width/12, height/7, 0, COLOR_SILVER));
	this->relpos.push_back(glm::vec3(0, 6*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_SILVER));
	this->relpos.push_back(glm::vec3(0, 4*height/14, 0));
	
	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_SILVER));
	this->relpos.push_back(glm::vec3(0, 2*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_SILVER));
	this->relpos.push_back(glm::vec3(0, 0, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_BLACK));
	this->relpos.push_back(glm::vec3(0, -6*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_BLACK));
	this->relpos.push_back(glm::vec3(0, -4*height/14, 0));
	
	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_SILVER));
	this->relpos.push_back(glm::vec3(0, -2*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_BLACK));
	this->relpos.push_back(glm::vec3(-2*width/14, -4*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_BLACK));
	this->relpos.push_back(glm::vec3(2*width/14, -4*height/14, 0));
}

void Sword::action(Player &player) {
	player.inv = std::clock();
	player.invincibility = true; 
}

Sworddisplay::Sworddisplay(float x, float y, float width, float height, float mass, color_t color) : Sword(x, y, width, height, mass, color) {

}

void Sworddisplay::tick(Player &player) {
	this->position.y = player.position.y;
	this->position.x = player.position.x + player.width/2;
	Sprite::tick();
}

Bolt::Bolt(float x, float y, float width, float height, float mass, color_t color) : Powerup(x, y, width, height, mass, color) {
	
	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_HARVESTGOLD));
	this->relpos.push_back(glm::vec3(0, -6*height/14, 0));
	
	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_HARVESTGOLD));
	this->relpos.push_back(glm::vec3(0, -4*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_HARVESTGOLD));
	this->relpos.push_back(glm::vec3(0, -2*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_HARVESTGOLD));
	this->relpos.push_back(glm::vec3(2*width/14, -2*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_HARVESTGOLD));
	this->relpos.push_back(glm::vec3(2*width/14, 0, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_HARVESTGOLD));
	this->relpos.push_back(glm::vec3(2*width/14, 2*height/14, 0));
	
	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_HARVESTGOLD));
	this->relpos.push_back(glm::vec3(4*width/14, 2*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_HARVESTGOLD));
	this->relpos.push_back(glm::vec3(4*width/14, 4*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, COLOR_HARVESTGOLD));
	this->relpos.push_back(glm::vec3(4*width/14, 6*height/14, 0));
}

void Bolt::action(Player &player){
	player.speedy += 0.001f;
}

Heart::Heart(float x, float y, float width, float height, float mass, color_t color) : Powerup(x, y, width, height, mass, color) {

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(-4*width/14, 2*height/14, 0));
	
	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(-4*width/14, -2*height/14, 0));
	
	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(-4*width/14, -0*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(0, 0, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(0, -6*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(0, -4*height/14, 0));
	
	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(0, -2*height/14, 0));
		
	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(-2*width/14, -2*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(-2*width/14, -0*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(-2*width/14, -4*height/14, 0));
	
	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(-2*width/14, 2*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(-2*width/14, -2*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(-2*width/14, -0*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(-2*width/14, -4*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(2*width/14, -2*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(2*width/14, -0*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(2*width/14, -4*height/14, 0));
	
	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(2*width/14, 2*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(2*width/14, -2*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(2*width/14, -0*height/14, 0));
	
	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(2*width/14, -4*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(4*width/14, 2*height/14, 0));
	
	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(4*width/14, -2*height/14, 0));

	this->recs.push_back(Rectangle(0, 0, width/7, height/7, 0, color));
	this->relpos.push_back(glm::vec3(4*width/14, 0*height/14, 0));
}

void Heart::action(Player &player) {
	player.lives++;
}


WaterBalloon::WaterBalloon(float x, float y, float width, float height, float mass, color_t color) : Sprite(x, y, width, height, 0, color) {
	this->mass = mass;
	this->momentum = glm::vec3(0.1,0,0);
	this->recs.push_back(Rectangle(0,0,width/4,height/4,0,color));
	this->relpos.push_back(glm::vec3(width/8,height/8,0));

	this->recs.push_back(Rectangle(0,0,width/4,height/4,0,color));
	this->relpos.push_back(glm::vec3(width/8,-height/8,0));

	this->recs.push_back(Rectangle(0,0,width/4,height/4,0,color));
	this->relpos.push_back(glm::vec3(3*width/8,height/8,0));

	this->recs.push_back(Rectangle(0,0,width/4,height/4,0,color));
	this->relpos.push_back(glm::vec3(3*width/8,-height/8,0));

	this->recs.push_back(Rectangle(0,0,width/4,height/4,0,color));
	this->relpos.push_back(glm::vec3(-width/8,0,0));
}

void WaterBalloon::tick() {
	float top    = screen_center_y + vertical_float / 1;
    float bottom = screen_center_y - vertical_float / 1;
    float left   = screen_center_x - horizontal_float / 1;
    float right  = screen_center_x + horizontal_float / 1;

	this->momentum.y -= 0.00125f;
    this->position += this->momentum / this->mass;

	Sprite::tick();
}

Viserion::Viserion(float x, float y, float width, float height, color_t color) : Sprite(x, y, width, height, 0, color) {
	
	this->mass = 1.0f;
	this->momentum = glm::vec3(0, 0, 0);

	this->recs.push_back(Rectangle(x,y,width/5,height/5,0,color));
	this->relpos.push_back(glm::vec3(-4*width/10,2*height/10,0));

	this->recs.push_back(Rectangle(x,y,width/5,height/5,0,color));
	this->relpos.push_back(glm::vec3(-2*width/10,2*height/10,0));

	this->recs.push_back(Rectangle(x,y,width/5,height/5,0,color));
	this->relpos.push_back(glm::vec3(-2*width/10,4*height/10,0));

	this->recs.push_back(Rectangle(x,y,width/5,height/5,0,color));
	this->relpos.push_back(glm::vec3(-2*width/10,-4*height/10,0));

	this->recs.push_back(Rectangle(x,y,width/5,height/5,0,color));
	this->relpos.push_back(glm::vec3(0*width/10,0*height/10,0));

	this->recs.push_back(Rectangle(x,y,width/5,height/5,0,color));
	this->relpos.push_back(glm::vec3(0*width/10,-2*height/10,0));

	this->recs.push_back(Rectangle(x,y,width/5,height/5,0,color));
	this->relpos.push_back(glm::vec3(2*width/10,0*height/10,0));

	this->recs.push_back(Rectangle(x,y,width/5,height/5,0,color));
	this->relpos.push_back(glm::vec3(2*width/10,-2*height/10,0));

	this->recs.push_back(Rectangle(x,y,width/5,height/5,0,color));
	this->relpos.push_back(glm::vec3(4*width/10,-4*height/10,0));
}

void Viserion::tick() {
	unsigned long ret = xorshf96();
	if(ret%2 == 0){
		this->momentum.x += 0.001f;
	}
	else{
		this->momentum.x -= 0.001f;
	}
	if(abs(this->momentum.x) > 0.005f){
		this->momentum.x = 0.005f*this->momentum.x/abs(this->momentum.x);
	}
	float top    = screen_center_y + vertical_float / 1;
    float bottom = screen_center_y - 9*vertical_float / (1*10);
	
	this->momentum.y -= 0.00125f;
    this->position += this->momentum / this->mass;
	if(this->position.y - this->height/2 < bottom){
		this->momentum.y *= (-1);
		this->position.y = bottom + this->height/2;
	}
    if(this->position.y + this->height/2 > top){
		this->momentum.y *= (-1);
		this->position.y = top - this->height/2;
	}

	Sprite::tick();

	add_iceballs(iceball_list, this->position.x, this->position.y);
}

Iceball::Iceball(float x, float y, float width, float height, float mass, color_t color) : Sprite(x, y, width, height, 0, color) {
	this->mass = mass;
	this->momentum = glm::vec3(-0.1f, 0, 0);
	
	this->recs.push_back(Rectangle(0,0,width/4,height/4,0,color));
	this->relpos.push_back(glm::vec3(width/8,height/8,0));

	this->recs.push_back(Rectangle(0,0,width/4,height/4,0,color));
	this->relpos.push_back(glm::vec3(width/8,-height/8,0));

	this->recs.push_back(Rectangle(0,0,width/4,height/4,0,color));
	this->relpos.push_back(glm::vec3(3*width/8,height/8,0));

	this->recs.push_back(Rectangle(0,0,width/4,height/4,0,color));
	this->relpos.push_back(glm::vec3(3*width/8,-height/8,0));

	this->recs.push_back(Rectangle(0,0,width/4,height/4,0,color));
	this->relpos.push_back(glm::vec3(-width/8,0,0));
}

void Iceball::tick() {
	float top    = screen_center_y + vertical_float / 1;
    float bottom = screen_center_y - vertical_float / 1;
    float left   = screen_center_x - horizontal_float / 1;
    float right  = screen_center_x + horizontal_float / 1;

	this->momentum.y -= 0.00125f;
    this->position += this->momentum / this->mass;
	for(int i = 0 ; i < this->recs.size() ; ++i) {
		if(xorshf96()%2 == 0)
			this->recs[i].rotation += 0.1f;
		else
			this->recs[i].rotation -= 0.1f;
	}
	Sprite::tick();

}

void Iceball::action(Player &player) {
	if(!player.inring && !player.invincibility)
		player.lives--;
}

Jetflare::Jetflare(float x, float y, float width, float height, float mass, color_t color) : Sprite(x, y, width, height, 0, color) {
	this->mass = mass;
	srand(std::time(0));
	this->momentum = glm::vec3( sin ( M_PI/(64*(1+(xorshf96())%7) )) ,-0.4f,0);
	this->start = std::clock();
	this->recs.push_back(Rectangle(x, y, width, height, 0, color));
	this->relpos.push_back(glm::vec3(0,0,0));
}

void Jetflare::tick() {
	this->position += this->momentum / this->mass;
	Sprite::tick();
}

Steam::Steam(float x, float y, float width, float height, float mass, color_t color) : Sprite(x, y, width, height, 0, color) {
	this->mass = mass;
	this->momentum = glm::vec3( sin ( M_PI/(64*(1+(xorshf96())%7) )) ,0.4f*sin( M_PI/(2*(1+(xorshf96())%7) )),0);
	this->start = std::clock();
	this->recs.push_back(Rectangle(x, y, width, height, 0, color));
	this->relpos.push_back(glm::vec3(0,0,0));
}

void Steam::tick() {
	this->position += this->momentum / this->mass;
	Sprite::tick();
}

Numbers::Numbers(float x, float y, float width, float height, string hash, color_t color) {
	Rectangle horizontal = Rectangle(0, 0, 8*width/10, height/10, 0, color);
	Rectangle vertical = Rectangle(0, 0, width/10, 7*height/20, 0, color);
	if(hash[0] == '1') {
		this->recs.push_back(horizontal);
		this->relpos.push_back(glm::vec3(x, y - height/2 + horizontal.height/2, 0));
	}
	if(hash[1] == '1') {
		this->recs.push_back(horizontal);
		this->relpos.push_back(glm::vec3(x, y, 0));
	}
	if(hash[2] == '1') {
		this->recs.push_back(horizontal);
		this->relpos.push_back(glm::vec3(x, y + height/2 - horizontal.height/2, 0));
	}
	if(hash[3] == '1') {
		this->recs.push_back(vertical);
		this->relpos.push_back(glm::vec3(x - width/2 + vertical.width/2, y - vertical.height/2, 0));
	}
	if(hash[4] == '1') {
		this->recs.push_back(vertical);
		this->relpos.push_back(glm::vec3(x + width/2 - vertical.width/2, y - vertical.height/2, 0));
	}
	if(hash[5] == '1') {
		this->recs.push_back(vertical);
		this->relpos.push_back(glm::vec3(x - width/2 + vertical.width/2, y + vertical.height/2, 0));
	}
	if(hash[6] == '1') {
		this->recs.push_back(vertical);
		this->relpos.push_back(glm::vec3(x + width/2 - vertical.width/2, y + vertical.height/2, 0));
	}
}

void Numbers::tick() {
	Sprite::tick();
}

CooldownBar::CooldownBar(float x, float y, float width, float height, color_t color) : Sprite(x, y, width, height, 0, color) {
	this->recs.push_back(Rectangle(x, y, width, height, 0, COLOR_RED));
	this->relpos.push_back(glm::vec3(0,0,0));
	this->recs.push_back(Rectangle(x, y, width, height, 0, color));
	this->relpos.push_back(glm::vec3(0,0,0));
}

void CooldownBar::tick(float tme) {
	this->recs.pop_back();
	this->relpos.pop_back();
	float w = min(this->width, this->width * (tme)/1.5f);
	float h = this->height;
	this->recs.push_back(Rectangle(this->position.x,this->position.y,w,h,0,this->color));
	this->relpos.push_back(glm::vec3(this->width/2 - w/2,0,0));
	this->position.x = screen_center_x;

	Sprite::tick();
}

Ring::Ring(float x, float y, float radius, float thick, color_t color) : Sprite(x, y, thick, thick, M_PI, COLOR_BLACK) {
	this->radius = radius;
	this->thick = thick;
	this->activated = false;
	this->recs.push_back(Rectangle(x, y, thick/10, thick/10, 0, color));
	this->relpos.push_back(glm::vec3(-radius, 0, 0));
	this->semis.push_back(Semicircle(x,y,radius,color));
	this->semis.push_back(Semicircle(x,y,radius-thick/2,COLOR_BACKGROUND));
}

void Ring::tick(Player &player, GLFWwindow *window) {

	float xdiff = this->position.x-player.position.x;
	float ydiff = this->position.y-player.position.y;

	if(sqrt(xdiff*xdiff+ydiff*ydiff)  > this->radius + 0.1f) {
		this->activated = false;
	}
	if( (this->rotation < 0 || this->rotation > M_PI || player.position.y < this->position.y - 0.5f) && player.inring) {
		this->activated = false;
		this->rotation = M_PI;
		player.inring = false;
	}
	if(this->activated == true) {

		player.inring = true;
		if(glfwGetKey(window, GLFW_KEY_LEFT)){
			this->rotation += 0.01f;
		}
		if(glfwGetKey(window, GLFW_KEY_RIGHT)){
			this->rotation -= 0.01f;
		}
		player.position.x = this->position.x + this->radius * cos(this->rotation);
		player.position.y = this->position.y + this->radius * sin(this->rotation);
	}
}

void Ring::draw(glm::mat4 VP){
	for(int i = 0 ; i < this->semis.size() ; ++i) {
		this->semis[i].draw(VP);
	}
	Sprite::draw(VP);
}

Semicircle::Semicircle(float x, float y, float radius, color_t color) {
	this->position = glm::vec3(x, y, 0);
	this->radius = radius;
	int n = 35;
	float angle = M_PI / n;
	float at = M_PI;
	GLfloat vertex_buffer_data [9*n];
	for(int i = 0 ; i < n ; ++i) {
		vertex_buffer_data[9*i] = 0.0f;
		vertex_buffer_data[9*i + 1] = 0.0f;
		vertex_buffer_data[9*i + 2] = 0.0f;
		vertex_buffer_data[9*i + 3] = this->radius*cos(at);
		vertex_buffer_data[9*i + 4] = this->radius*sin(at);
		at -= angle;
		vertex_buffer_data[9*i + 5] = 0.0f;
		vertex_buffer_data[9*i + 6] = this->radius*cos(at);
		vertex_buffer_data[9*i + 7] = this->radius*sin(at);
		vertex_buffer_data[9*i + 8] = 0.0f;
	}
	this->object = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data, color, GL_FILL);
}

void Semicircle::draw(glm::mat4 VP){
	Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

Firebeamconfusion::Firebeamconfusion(float x, float y, float width, float height, color_t color) : Sprite(x, y, width, height, 0, color) {
	this->start = std::clock();
	this->position = glm::vec3(x, y, 0);
	this->recs.push_back(Rectangle(0,0,0,0.25f,0,COLOR_FIRERED));
	this->relpos.push_back(glm::vec3(0,3*vertical_float,0));
	this->recs.push_back(Rectangle(0,0,0.5f,0.5f,M_PI/4,COLOR_FERRARIRED));
	this->relpos.push_back(glm::vec3(-width/2,0,0));	
	this->recs.push_back(Rectangle(0,0,0.5f,0.5f,M_PI/4,COLOR_FERRARIRED));
	this->relpos.push_back(glm::vec3(width/2,0,0));
}

void Firebeamconfusion::tick(Player &player) {
	this->position.x = player.position.x;
	float tme = (std::clock() - this->start)/(float)CLOCKS_PER_SEC;
	if( tme < 8.0f && tme > 4.0f) {
		this->recs[0] = Rectangle(0,0,this->width*(tme-4.0f)/4.0f,0.25f,0,COLOR_CHROMEYELLOW);
		this->relpos[0].y = 0;
	}
	if(tme > 4.0f) {
		add_jetflares(jetflare_list,this->position.x - this->recs[0].width/2, this->position.y);
		add_jetflares(jetflare_list,this->position.x + this->recs[0].width/2, this->position.y);
	}
	Sprite::tick();
}

void Firebeamconfusion::action(Player &player) {
	if(!player.inring && !player.invincibility){
		player.lives--;
	}
}