#ifndef LIGHT_H
#define LIGHT_H

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

struct sLightLocation{
    GLuint ambientIntensityLocation = 0;
    GLuint ambientCoulourLocation = 0;
    GLuint diffuseIntensityLocation = 0;
    GLuint directionLocation = 0;
    GLuint positionLocation = 0;
    GLuint constantLocation = 0;
    GLuint linearLocation = 0;
    GLuint exponentialLocation = 0;
    GLuint edgeLocation = 0;
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