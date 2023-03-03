#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#pragma once

#include "Light.h"

class PointLight:public Light
{
public:
    PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat di,
        glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat exp
    );
    ~PointLight(){};
    virtual void use(sLightLocation lightloc)override;

private:
    glm::vec3 position;
    GLfloat constant, linear, exponent;// attenuation ax*x + bx + c
};

#endif