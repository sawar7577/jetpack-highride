#include "ball.h"
#include "main.h"
// #include <bits/stdc++.h>

Ball::Ball(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    int n = 40;
    GLfloat vertex_buffer_data[9*n]; 
	
	double angle = (double)(2*M_PI)/(double)(n);
	double arr[3] = {0.0 , 1.0, 0.0};
	double temp[3];
	double zero[3] = {0.0, 0.0, 0.0};
	for(int i = 0, j = 0; i < 3*n; ++i)
	{
		if(i%3 == 0)
		{
			vertex_buffer_data[j++] = zero[0];
			vertex_buffer_data[j++] = zero[1];
			vertex_buffer_data[j++] = zero[2];
		}
		else if(i%3 == 1)
		{
			vertex_buffer_data[j++] = arr[0];
			vertex_buffer_data[j++] = arr[1];
			vertex_buffer_data[j++] = arr[2];	
		}
		else
		{
			temp[0] = arr[0]*cos(angle) - arr[1]*sin(angle);
			temp[1] = arr[0]*sin(angle) + arr[1]*cos(angle);
			temp[2] = 0.0;
			arr[0] = temp[0];
			arr[1] = temp[1];
			arr[2] = temp[2];

			vertex_buffer_data[j++] = arr[0];
			vertex_buffer_data[j++] = arr[1];
			vertex_buffer_data[j++] = arr[2];	
		}
	}


    this->object = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model * glm::translate(glm::vec3(1,0,0));
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this->rotation += speed;
}

Player::Player(float x, float y, float mass, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->mass = mass;
    this->momentum = glm::vec3(0, 0, 0);
    int n = 40;
    GLfloat vertex_buffer_data[9*n]; 
	
	double angle = (double)(2*M_PI)/(double)(n);
	double arr[3] = {0.0 , 1.5, 0.0};
	double temp[3];
	double zero[3] = {0.0, 0.0, 0.0};
	for(int i = 0, j = 0; i < 3*n; ++i)
	{
		if(i%3 == 0)
		{
			vertex_buffer_data[j++] = zero[0];
			vertex_buffer_data[j++] = zero[1];
			vertex_buffer_data[j++] = zero[2];
		}
		else if(i%3 == 1)
		{
			vertex_buffer_data[j++] = arr[0];
			vertex_buffer_data[j++] = arr[1];
			vertex_buffer_data[j++] = arr[2];	
		}
		else
		{
			temp[0] = arr[0]*cosf(angle) - arr[1]*sinf(angle);
			temp[1] = arr[0]*sinf(angle) + arr[1]*cosf(angle);
			temp[2] = 0.0;
			arr[0] = temp[0];
			arr[1] = temp[1];
			arr[2] = temp[2];

			vertex_buffer_data[j++] = arr[0];
			vertex_buffer_data[j++] = arr[1];
			vertex_buffer_data[j++] = arr[2];	
		}
	}


    this->object = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data, color, GL_FILL);
}

void Player::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Player::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

// float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
// float camera_rotation_angle = 0;

void Player::tick(GLFWwindow *window) {
    // this->rotation += speed;

    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;

    this->momentum.y -= 0.04f;
    this->momentum.x += (this->momentum.x*(-0.08f));
        
    if(glfwGetKey(window,GLFW_KEY_LEFT)){
        this->momentum.x -= 0.09f;
    }
    if(glfwGetKey(window,GLFW_KEY_RIGHT)){
        this->momentum.x += 0.09f;
    }
    if(glfwGetKey(window,GLFW_KEY_UP)){
        this->momentum.y += 0.09f;
    }
    if(glfwGetKey(window,GLFW_KEY_DOWN)){
        this->momentum.y -= 0.09f;
    }
    // std::cout << this->momentum.x << std::endl;
    this->position += this->momentum / this->mass;

	if(this->position.y <= bottom){
		this->momentum.y = 0.0f;
		this->position.y = bottom;
	}
    if(this->position.y >= top){
		this->momentum.y = 0.0f;
		this->position.y = top;
	}
    
	screen_center_x = this->position.x;
}

Rectangle::Rectangle(float x, float y, float w, float h, float rot, color_t color) {
	this->position = glm::vec3(x,y,0);
    this->rotation = rot;
	std::cout << this->rotation << std::endl;

	// static const GLfloat vertex_buffer_data [] = {
	// 	0.0f, 0.0f, 0.0f,
	// 	0.0f, 0.0f + height, 0.0f,
	// 	0.0f + width, 0.0f + height, 0.0f,
	// 	0.0f, 0.0f, 0.0f,
	// 	0.0f + width, 0.0f, 0.0f,
	// 	0.0f + width, 0.0f + height, 0.0f,
	// };
	float width = w;
	float height = h;
	GLfloat vertex_buffer_data [] = {
		0.0f - width/2.0f, 0.0f - height/2.0f, 0.0f,
		0.0f - width/2.0f, 0.0f + height/2.0f, 0.0f,
		0.0f + width/2.0f, 0.0f + height/2.0f, 0.0f,
		0.0f - width/2.0f, 0.0f - height/2.0f, 0.0f,
		0.0f + width/2.0f, 0.0f - height/2.0f, 0.0f,
		0.0f + width/2.0f, 0.0f + height/2.0f, 0.0f,
	};
	// std::cout << vertex_buffer_data[0] << " " << vertex_buffer_data[1] << std::endl;
	this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
}

void Rectangle::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
	// this->rotation = 0;
    glm::mat4 rotate    = glm::rotate((float) (this->rotation), glm::vec3(0, 0, 1));
	// No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

Floor::Floor(float x, float y){
	map = 0;
	Rectangle t;
	
	for(int i = 0 ; i < 20 ; ++i){
		if(i%2 == 0) {
			t = Rectangle(x, y, 1.0f, 1.0f, 0, COLOR_RED);
		}
		else {
			t = Rectangle(x, y, 1.0f, 1.0f, 0, COLOR_GREEN);
		}
		t.position.x += i*(1.0f);
		// std::cout << t.position.x << std::endl;

		tiles.push_back(t);
	}
}

void Floor::draw(glm::mat4 VP, float x) {
	// std::cout << tiles[map].position.x << std::endl;

	if(tiles[map].position.x - x < -8.0f){
		tiles[map].position.x = tiles[ (map+19)%20 ].position.x+1.0f;
		map++; 
		// std::cout << "here" << std::endl;
	}
	else{
		tiles[(map+19)%20].position.x = tiles[ map ].position.x-1.0f;	
		map--;
		// std::cout << "here" << std::endl;
	}
	map = (map+20)%20;
	for(int i = 0 ; i < 20 ; ++i){
		tiles[i].draw(VP);
	}
}

Magnet::Magnet(float x, float y, color_t color){
	this->position = glm::vec3(x, y, 0);
	this->force = 1.0f;
	float height = 1.0f;
	float width  = 1.0f;
	// static const GLfloat vertex_buffer_data [] = {
	// 	0.0f, 0.0f, 0.0f,
	// 	0.0f, 0.0f + height, 0.0f,
	// 	0.0f + width, 0.0f + height, 0.0f,
	// 	0.0f, 0.0f, 0.0f,
	// 	0.0f + width, 0.0f, 0.0f,
	// 	0.0f + width, 0.0f + height, 0.0f,
	// };
	static const GLfloat vertex_buffer_data [] = {
		0.0f - width/2, 0.0f - height/2, 0.0f,
		0.0f - width/2, 0.0f + height/2, 0.0f,
		0.0f + width/2, 0.0f + height/2, 0.0f,
		0.0f - width/2, 0.0f - height/2, 0.0f,
		0.0f + width/2, 0.0f - height/2, 0.0f,
		0.0f + width/2, 0.0f + height/2, 0.0f,
	};
	this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);	
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    // glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Magnet::tick(Player &player) {
	glm::vec3 dist = this->position - player.position;
	if(dist.x*dist.x + dist.y*dist.y >= 2.0f){
	float ef = (this->force/player.mass)/(dist.x*dist.x+dist.y*dist.y);
	glm::vec3 ch(ef*dist.x/sqrt(dist.x*dist.x+dist.y*dist.y),ef*dist.y/sqrt((dist.x*dist.x+dist.y*dist.y)),0);
	player.momentum.x += ch.x;
	player.momentum.y += ch.y;
	// std::cout << ch.x << " " << ch.y << std::endl;
	}
}

Fireline::Fireline(float x, float y, float length, float rot, color_t color) {
	this->position = glm::vec3(x, y, 0);
	// this->rotation = rot;
	// std::cout << length << " length" << std::endl;
	this->r1 = Rectangle(x,y,length,0.5f,rot,color);
	this->r2 = Rectangle((x)+cos(rot)*length/2.0f,y+(sin(rot))*length/2.0f,1.0f,1.0f,rot,color);
	this->r3 = Rectangle((x)+cos(rot+M_PI)*length/2.0f,y+(sin(rot+M_PI))*length/2.0f,1.0f,1.0f,rot,color);

	// static const GLfloat vertex_buffer_data [] = {
	// 	0.0f, 0.0f, 0.0f,
	// 	0.0f, 0.0f + height, 0.0f,
	// 	0.0f + width, 0.0f + height, 0.0f,
	// 	0.0f, 0.0f, 0.0f,
	// 	0.0f + width, 0.0f, 0.0f,
	// 	0.0f + width, 0.0f + height, 0.0f,
	// };
	// this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);	
}

void Fireline::draw(glm::mat4 VP) {
    // glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // Matrices.model = glm::mat4(1.0f);
    // glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    // // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    // Matrices.model *= (translate*rotate);
    // glm::mat4 MVP = VP * Matrices.model;
    // glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    // draw3DObject(this->object);
	this->r1.draw(VP);
	this->r2.draw(VP);
	this->r3.draw(VP);
}