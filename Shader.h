#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <GL/glew.h>

class Shader
{
public:
    Shader():id(0),uniformModel(0),uniformProjection(0),uniformView(0){}
    ~Shader(){clear();}

    void createFromFiles(std::string vShader , std::string fShader);
    GLuint getProjectionLocation();
    GLuint getModelLocation();
    GLuint getViewLocation();
    GLuint getAmbientIntensityLocation();
    GLuint getAmbientColourLocation();
    GLuint getDiffuseIntensityLocation();
    GLuint getDirectionLocation();
    GLuint getSpecularIntensityLocation();
    GLuint getShininessLocation();
    GLuint getEyePositionLocation();

    void use();
    void clear();
private:
    GLuint id, uniformModel, uniformProjection,uniformView,uniformEyePos,
        uniformAmbientColour,uniformAmbientIntensity,
        uniformDiffuseIntensity,uniformDirection,
        uniformSpecularIntensity,uniformShininess;
    void compile(std::string vcode , std::string fcode);
    void add( GLuint theProgram,const std::string shaderCode, GLenum shaderType);
};

#endif