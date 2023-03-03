#ifndef LIGHT_H
#define LIGHT_H

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

struct sLightLocation{
    GLfloat ambientIntensityLocation = 0;
    GLfloat ambientCoulourLocation = 0;
    GLfloat diffuseIntensityLocation = 0;
    GLfloat directionLocation = 0;
    GLfloat positionLocation = 0;
    GLfloat constantLocation = 0;
    GLfloat linearLocation = 0;
    GLfloat exponentialLocation = 0;
};

class Light
{
public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat di);
    ~Light();

    virtual void use(sLightLocation lightLoc);

protected:
    glm::vec3 colour;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;

};

#endif