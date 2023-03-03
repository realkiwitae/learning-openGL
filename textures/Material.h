#ifndef MATERIAL_H
#define MATERIAL_H

#pragma once

#include <GL/glew.h>

class Material
{
private:
    GLfloat specularIntensity;
    GLfloat shininess;

public:
    Material():specularIntensity(0.f),shininess(0.f){}
    Material(GLfloat si, GLfloat s):specularIntensity(si),shininess(s){}
    ~Material(){}

    void use(GLuint specularIntensityLocation, GLuint shininessLocation);


};

#endif