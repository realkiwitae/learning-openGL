#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#pragma once

#include "./Light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight();
    ~DirectionalLight(){}
    DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat di,glm::vec3 dir);

    virtual void use(sLightLocation lightloc)override;
private:
    glm::vec3 direction;

};

#endif