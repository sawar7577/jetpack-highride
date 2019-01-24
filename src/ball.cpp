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


void add_coin(list <Ball> &l) {
    for(int i = 0 ; i < 1 ; ++i) {
        int random = rand();
        random = -4000 + random % (7999);
        if(random%7 == 0){
            l.push_back(Ball(screen_center_x + 2*horizontal_float, (float)(random)/1000.0f ,COLOR_FIREYELLOW));
        }
    }
}

unsigned long xorshf96(void) {          //period 2^96-1
	static unsigned long x=123456789, y=362436069, z=521288629;
	unsigned long t;
		x ^= x << 16;
		x ^= x >> 5;
		x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
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

Sprite::Sprite(float x, float y, float width, float height, float rotation, color_t color) {
	this->position = glm::vec3(x, y, 0);
	this->width = width;
	this->height = height;
	this->rotation = rotation;
	this->color = color;
}

void Sprite::draw(glm::mat4 VP) {
	for(int i = 0 ; i < this->recs.size() ; ++i) {
		this->recs[i].draw(VP);
	}
}

void Sprite::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

Ball::Ball(float x, float y, color_t color) : Sprite(x, y , 0.25, 0.25, M_PI/4, COLOR_FIREYELLOW){
	this->recs.push_back(Rectangle(x, y, 0.25f, 0.25f, M_PI/4, COLOR_FIREYELLOW));
}

void Ball::tick() {
    // this->rotation += 0.9999999f;
	// for(int i = 0 ; i < this->recs.size() ; ++i){
		// this->recs[i].rotation = this->rotation;
	// }
	// std::cout << this->rotation << std::endl;
}

Player::Player(float x, float y, float width, float height, float mass, color_t color):Sprite(x,y,width,height,0,color) {
	this->mass = mass;
	this->momentum = glm::vec3(0, 0, 0);
	this->cooldown = std::clock();
	
	this->recs.push_back(Rectangle(x+width/2-width/3,y-height/10,2*width/3,height/2,0,COLOR_GREEN));
	this->relpos.push_back(glm::vec3(width/2-width/3,-height/10,0));
	this->recs.push_back(Rectangle(x-width/2+width/6,y,width/3,height/3,0,COLOR_RED));
	this->relpos.push_back(glm::vec3(-width/2+width/6,0,0));	
	this->recs.push_back(Rectangle(x-width/2+width/6,y+height/8,width/5,2*height/8,0,COLOR_RED));
	this->relpos.push_back(glm::vec3(-width/2+width/6,height/8,0));	
	this->recs.push_back(Rectangle(x+width/2-width/3+width/5,y-height/4-height/8,width/5,height/4,0,COLOR_RED));
	this->relpos.push_back(glm::vec3(width/2-width/3+width/5,-height/4-height/8,0));	
	this->recs.push_back(Rectangle(x+width/2-width/3-width/5,y-height/4-height/8,width/5,height/4,0,COLOR_RED));
	this->relpos.push_back(glm::vec3(width/2-width/3-width/5,-height/4-height/8,0));	
	this->recs.push_back(Rectangle(x+width/2-width/3,y-height/10+height/4+height/40,width/8,height/20,0,COLOR_RED));
	this->relpos.push_back(glm::vec3(width/2-width/3,-height/10+height/4+height/40,0));	
	this->recs.push_back(Rectangle(x+width/2-width/3,y-height/10+height/4+height/20+13*height/80,2*width/3,13*height/40,0,COLOR_BLACK));
	this->relpos.push_back(glm::vec3(width/2-width/3,-height/10+height/4+height/20+13*height/80,0));
}


void Player::tick(GLFWwindow *window) {

    float top    = screen_center_y + vertical_float / screen_zoom;
    float bottom = screen_center_y - vertical_float / screen_zoom;
    float left   = screen_center_x - horizontal_float / screen_zoom;
    float right  = screen_center_x + horizontal_float / screen_zoom;

    // this->momentum.x += (this->momentum.x*(-0.01f));
    	if(glfwGetKey(window,GLFW_KEY_LEFT)){
        this->momentum.x -= 0.0036f;
    }
    if(glfwGetKey(window,GLFW_KEY_RIGHT)){
        this->momentum.x += 0.0036f;
		add_coin(ball_list);
    }
    if(glfwGetKey(window,GLFW_KEY_UP)){
		add_jetflares(jetflare_list, this->position.x - this->width/2, this->position.y);
        this->momentum.y += 0.0036f;
    }
    if(glfwGetKey(window,GLFW_KEY_DOWN)){
        this->momentum.y -= 0.0036f;
    }
	if(glfwGetKey(window,GLFW_KEY_SPACE)){
		clock_t now = std::clock();
		if( (now-this->cooldown)/(double)CLOCKS_PER_SEC > 1.5f) {
			waterballoon_list.push_back(WaterBalloon(this->position.x, this->position.y, 0.5f, 0.5f, 1.0f, COLOR_GREEN));
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
	for(int i = 0 ; i < recs.size() ; ++i) {
		this->recs[i].position = this->position + this->relpos[i];
	}
    
	screen_center_x = this->position.x;
}

Rectangle::Rectangle(float x, float y, float w, float h, float rot, color_t color) {
	this->position = glm::vec3(x,y,0);
    this->rotation = rot;
	this->width = w;
	this->height = h;

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
	// No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

Floor::Floor(float x, float y):Sprite(x,y,0,0,0,COLOR_BLACK){
	this->map = 0;
	Rectangle t;
	for(int i = 0 ; i < 20 ; ++i){
		if(i%2 == 0) {
			t = Rectangle(x, y, 1.0f, 1.0f, 0, COLOR_RED);
		}
		else {
			t = Rectangle(x, y, 1.0f, 1.0f, 0, COLOR_GREEN);
		}
		t.position.x += i*(1.0f);
		this->recs.push_back(t);
	}
}

void Floor::draw(glm::mat4 VP, float x) {
	if(this->recs[this->map].position.x - x < -8.0f){
		this->recs[this->map].position.x = this->recs[ (this->map+19)%20 ].position.x+1.0f;
		this->map++; 
	}
	else{
		this->recs[(this->map+19)%20].position.x = this->recs[ this->map ].position.x-1.0f;	
		this->map--;
	}
	this->map = (this->map+20)%20;
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

Fireline::Fireline(float x, float y, float length, float rot, color_t color) {
	this->position = glm::vec3(x, y, 0);
	this->recs.push_back(Rectangle(x,y,length,0.25f,rot,COLOR_FIRERED));
	this->recs.push_back(Rectangle((x)+cos(rot)*length/2.0f,y+(sin(rot))*length/2.0f,0.5f,0.5f,rot+M_PI/4,COLOR_FIREYELLOW));
	this->recs.push_back(Rectangle((x)+cos(rot+M_PI)*length/2.0f,y+(sin(rot+M_PI))*length/2.0f,0.5f,0.5f,rot+M_PI/4,COLOR_FIREYELLOW));	
}

Firebeam::Firebeam(float x, float y, float length, float rot, color_t color) {
	this->position = glm::vec3(x, y, 0);
	this->speed = 0.015f;
	this->recs.push_back(Rectangle(x,y,length,0.25f,rot,COLOR_FIRERED));
	this->recs.push_back(Rectangle((x)+cos(rot)*length/2.0f,y+(sin(rot))*length/2.0f,0.5f,0.5f,rot+M_PI/4,COLOR_FIREYELLOW));
	this->recs.push_back(Rectangle((x)+cos(rot+M_PI)*length/2.0f,y+(sin(rot+M_PI))*length/2.0f,0.5f,0.5f,rot+M_PI/4,COLOR_FIREYELLOW));	
}

void Firebeam::tick() {
	float top    = screen_center_y + vertical_float / screen_zoom;
    float bottom = screen_center_y - vertical_float / screen_zoom;
    float left   = screen_center_x - horizontal_float / screen_zoom;
    float right  = screen_center_x + horizontal_float / screen_zoom;

	this->position.y += this->speed;
	for(int i = 0 ; i < this->recs.size() ; ++i) {
		this->recs[i].position.y += this->speed;
	}
	if(this->position.y > top ) {
		this->speed *= -1;
		for(int i = 0 ; i < this->recs.size() ; ++i) {
			this->recs[i].position.y = top;
		}
	}
	if( this->position.y < bottom) {
		this->speed *= -1;
		for(int i = 0 ; i < this->recs.size() ; ++i) {
			this->recs[i].position.y = bottom;
		}
	}
}

Boomerang::Boomerang(float x, float y, float center_x, float center_y, float width, float height, color_t color) : Sprite(x, y, width, height, 0, color) {
	this->center_x = center_x;
	this->center_y = center_y;
	this->recs.push_back(Rectangle(x + width/2 - sqrt(width*width+height*height)/4*2, y + height/2 - sqrt(width*width+height*height)/4*2, width/4, height/4, M_PI/4, color));
	this->relpos.push_back(glm::vec3(width/2 - sqrt(width*width+height*height)/4*2, height/2 - sqrt(width*width+height*height)/4*2, 0));
	this->recs.push_back(Rectangle(this->recs[0].position.x - sqrt(width*width+height*height)/4*2, this->recs[0].position.y, width/4, height/4, M_PI/4, color));
	this->relpos.push_back(glm::vec3(width/2 - sqrt(width*width+height*height)/4, height/2 - sqrt(width*width+height*height)/4*2, 0));	
	this->recs.push_back(Rectangle(this->recs[0].position.x, this->recs[0].position.y - sqrt(width*width+height*height)/4*2, width/4, height/4, M_PI/4, color));
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
	this->position.x = 4.0f*cos(this->rotation) + this->center_x;
	this->position.y = 2.25f*sin(this->rotation) + this->center_y;
	for(int i = 0 ; i < this->recs.size() ; ++i) {
		this->recs[i].rotation += 0.25f;
		this->relpos[i] = rotateaxes(this->relpos[i],0.25f);
		this->recs[i].position = this->relpos[i] + this->position;

	}
}

Powerup::Powerup(float x, float y, float width, float height, float mass, color_t color) : Sprite(x, y, width, height, 0, color) {
	this->mass = mass;
	this->momentum = glm::vec3(-0.1,0,0);
	this->recs.push_back(Rectangle(x, y, width, height, 0, color));
	this->relpos.push_back(glm::vec3(0, 0, 0));
}

void Powerup::tick() {
	float top    = screen_center_y + vertical_float / screen_zoom;
    float bottom = screen_center_y - vertical_float / screen_zoom;
    float left   = screen_center_x - horizontal_float / screen_zoom;
    float right  = screen_center_x + horizontal_float / screen_zoom;

	this->momentum.y -= 0.00125f;
    this->position += this->momentum / this->mass;
	if(this->position.y < bottom){
		this->momentum.y *= (-1);
		this->position.y = bottom;
	}
    if(this->position.y  > top){
		this->momentum.y *= (-1);
		this->position.y = top;
	}
	for(int i = 0 ; i < recs.size() ; ++i) {
		this->recs[i].position = this->position + this->relpos[i];
	}

}

WaterBalloon::WaterBalloon(float x, float y, float width, float height, float mass, color_t color) : Sprite(x, y, width, height, 0, color) {
	this->mass = mass;
	this->momentum = glm::vec3(0.1,0,0);
	this->recs.push_back(Rectangle(x, y, width, height, 0, color));
	this->relpos.push_back(glm::vec3(0, 0, 0));
}

void WaterBalloon::tick() {
	float top    = screen_center_y + vertical_float / screen_zoom;
    float bottom = screen_center_y - vertical_float / screen_zoom;
    float left   = screen_center_x - horizontal_float / screen_zoom;
    float right  = screen_center_x + horizontal_float / screen_zoom;

	this->momentum.y -= 0.00125f;
    this->position += this->momentum / this->mass;
	for(int i = 0 ; i < recs.size() ; ++i) {
		this->recs[i].position = this->position + this->relpos[i];
	}
}

Viserion::Viserion(float x, float y, float width, float height, color_t color) : Sprite(x, y, width, height, 0, color) {
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
	for(int i = 0 ; i < this->recs.size() ; ++i) {
		this->recs[i].position = this->position + this->relpos[i];
	}
}

Iceball::Iceball(float x, float y, float width, float height, float mass, color_t color) : Sprite(x, y, width, height, 0, color) {
	this->mass = mass;
	
}

void Iceball::tick() {
	float top    = screen_center_y + vertical_float / screen_zoom;
    float bottom = screen_center_y - vertical_float / screen_zoom;
    float left   = screen_center_x - horizontal_float / screen_zoom;
    float right  = screen_center_x + horizontal_float / screen_zoom;

	this->momentum.y -= 0.00125f;
    this->position += this->momentum / this->mass;
	for(int i = 0 ; i < recs.size() ; ++i) {
		this->recs[i].position = this->position + this->relpos[i];
	}	
}

Jetflare::Jetflare(float x, float y, float width, float height, float mass, color_t color) : Sprite(x, y, width, height, 0, color) {
	this->mass = mass;
	srand(std::time(0));
	this->momentum = glm::vec3( sin ( M_PI/(64*(1+(xorshf96())%7) )) ,-0.4f,0);
	this->start = std::clock();
	this->recs.push_back(Rectangle(x, y, width, height, 0, color));
}

void Jetflare::tick() {
	for(int i = 0 ; i < this->recs.size() ; ++i) {
		this->recs[i].position += this->momentum / this->mass;
	}
}

Steam::Steam(float x, float y, float width, float height, float mass, color_t color) : Sprite(x, y, width, height, 0, color) {
	this->mass = mass;
	this->momentum = glm::vec3( sin ( M_PI/(64*(1+(xorshf96())%7) )) ,0.4f,0);
	this->start = std::clock();
	this->recs.push_back(Rectangle(x, y, width, height, 0, color));
}

void Steam::tick() {

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
	for(int i = 0 ; i < this->recs.size() ; ++i) {
		this->recs[i].position = this->position + this->relpos[i];
	}
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
	std::cout << std::setprecision(10);
	std::cout << this->width << " " <<  this->width * tme / 0.5f << std::endl;;
	float h = this->height;
	this->recs.push_back(Rectangle(this->position.x,this->position.y,w,h,0,this->color));
	this->relpos.push_back(glm::vec3(this->width/2 - w/2,0,0));

	for(int i = 0 ; i < this->recs.size() ; ++i) {
		this->recs[i].position = this->position + this->relpos[i];
	}
}