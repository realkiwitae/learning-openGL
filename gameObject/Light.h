#ifndef LIGHT_H
#define LIGHT_H

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,glm::vec3 dir, GLfloat di);
    ~Light();

    void use(GLfloat ambientIntensityLocation, GLfloat ambientCoulourLocation,
        GLfloat diffuseIntensityLocation,GLfloat directionLocation
    );

private:
    glm::vec3 colour;
    GLfloat ambientIntensity;

    glm::vec3 direction;
    GLfloat diffuseIntensity;
};

#endif