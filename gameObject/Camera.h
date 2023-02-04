#ifndef CAMERA_H
#define CAMERA_H

#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(glm::vec3 _p,glm::vec3 _up,GLfloat _y, GLfloat _pitch,GLfloat sms,GLfloat tms):position(_p),front(glm::vec3(0,0,-1)),worldup(_up),yaw(_y),pitch(_pitch),movespeed(sms),turnspeed(tms){update();}
    Camera():position(glm::vec3(0,0,0)),front(glm::vec3(0,0,-1)),worldup(glm::vec3(0,1,0)),yaw(0),pitch(0),movespeed(0),turnspeed(0){update();}

    ~Camera(){};

    void keyControls(long* keys);
    void mouseControl(GLfloat xchange, GLfloat ychange);
    glm::mat4 calculateViewMatrix();
private:

    void update();

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldup;

    GLfloat yaw,pitch;
    GLfloat movespeed;
    GLfloat turnspeed;
};

#endif