#ifndef LIGHT_H
#define LIGHT_H

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);
    ~Light();

    void use(GLfloat ambientIntensityLocation, GLfloat ambientCoulourLocation);

private:
    glm::vec3 colour;
    GLfloat ambientIntensity;
};

#endif