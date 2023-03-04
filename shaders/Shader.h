#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <GL/glew.h>

#include "../gameObject/DirectionalLight.h"
#include "../gameObject/PointLight.h"
#include "../gameObject/SpotLight.h"

#include "../utils/utils.h"

class Shader
{
public:
    Shader():id(0),uniformModel(0),uniformProjection(0),uniformView(0),n_pointLights(0){}
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
    void setDirectionalLight(DirectionalLight* dLight);
    void setPointLights(PointLight* pLights, unsigned int n);
    void setSpotLights(SpotLight* sLights, unsigned int n);
private:
    int n_pointLights;
    int n_spotLights;

    struct {
        GLuint colour;
        GLuint ambientIntensity;
        GLuint diffuseIntensity;
        GLuint direction;
    } uniformDirectionLight;
    struct {
        GLuint colour;
        GLuint ambientIntensity;
        GLuint diffuseIntensity;
        GLuint position;
        GLuint constant;
        GLuint linear;
        GLuint exponent;
    } uniformPointLight[MAX_N_POINT_LIGHTS];
    struct {
        GLuint colour;
        GLuint ambientIntensity;
        GLuint diffuseIntensity;
        GLuint position;
        GLuint constant;
        GLuint linear;
        GLuint exponent;
        GLuint direction;
        GLuint edge;
    } uniformSpotLight[MAX_N_SPOT_LIGHTS];

    GLuint uniform_n_pointLights;
    GLuint uniform_n_spotLights;
    GLuint id, uniformModel, uniformProjection,uniformView,uniformEyePos,
        uniformSpecularIntensity,uniformShininess;
    void compile(std::string vcode , std::string fcode);
    void add( GLuint theProgram,const std::string shaderCode, GLenum shaderType);
};

#endif