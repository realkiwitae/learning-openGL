#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#pragma once

#include "PointLight.h"

class SpotLight : public PointLight
{
public:
    SpotLight();
    ~SpotLight(){}
    SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat di,
        glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat exp,
         glm::vec3 dir, GLfloat edg 
    );
    virtual void use(sLightLocation lightloc)override;
    void setFlash(glm::vec3 pos, glm::vec3 dir);
private:
    glm::vec3 direction;
    GLfloat edge,procEdge;
};

#endif