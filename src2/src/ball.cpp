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
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
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
    this->momentum.x += (this->momentum.x*(-0.1f));
        
    if(glfwGetKey(window,GLFW_KEY_LEFT)){
        this->momentum.x -= 0.05f;
    }
    if(glfwGetKey(window,GLFW_KEY_RIGHT)){
        this->momentum.x += 0.05f;
    }
    if(glfwGetKey(window,GLFW_KEY_UP)){
        this->momentum.y += 0.09f;
    }
    if(glfwGetKey(window,GLFW_KEY_DOWN)){
        this->momentum.y -= 0.09f;
    }
    // std::cout << this->momentum.x << std::endl;
    this->position += this->momentum / this->mass;


    this->position.y < bottom ? this->momentum.y = 0.0f : this->momentum.y = this->momentum.y ;    
    this->position.y < bottom ? this->position.y = bottom : this->position.y = this->position.y ;
    
    this->position.y > top ? this->momentum.y = 0.0f : this->momentum.y = this->momentum.y ;        
    this->position.y > top ? this->position.y = top : this->position.y = this->position.y ;

}

