#include "Camera.h"
#include "../utils/utils.hpp"
#include "GLFW/glfw3.h"

#include <iostream>

extern double now,last_time,delta_time;

void Camera::update(){
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldup));
	up = glm::normalize(glm::cross(right, front));

}

void Camera::keyControls(long* keys){
    if(CHECK_BIT(*keys,GLFW_KEY_W)){
        position += front*movespeed*(GLfloat)delta_time;
    }
    if(CHECK_BIT(*keys,GLFW_KEY_S)){
        position -= front*movespeed*(GLfloat)delta_time;
    }
    if(CHECK_BIT(*keys,GLFW_KEY_A)){
        position -= right*movespeed*(GLfloat)delta_time;
    }
    if(CHECK_BIT(*keys,GLFW_KEY_D)){
        position += right*movespeed*(GLfloat)delta_time;
    }   
}

glm::mat4 Camera::calculateViewMatrix(){
    return glm::lookAt(position, position + front, up);
}

void Camera::mouseControl(GLfloat xchange, GLfloat ychange)
{

    xchange *= turnspeed;
    ychange *= turnspeed;

    yaw += xchange;
    pitch += ychange;

    pitch = glm::clamp(pitch,-89.f,89.f);

    update();
}

glm::vec3 Camera::getPosition(){return position;}