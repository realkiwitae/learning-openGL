#include "Light.h"

Light::Light():colour(1.f,1.f,1.f),ambientIntensity(1.f),
direction(glm::vec3(0.f,-1.f,0.f)),diffuseIntensity(0)
{

}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, glm::vec3 dir, GLfloat di)
:colour(red,green,blue),ambientIntensity(aIntensity),direction(glm::vec3(dir.x,dir.y,dir.z)),diffuseIntensity(di)
{

}

void Light::use(GLfloat ambientIntensityLocation, GLfloat ambientCoulourLocation, GLfloat diffuseIntensityLocation,GLfloat directionLocation){
    glUniform3f(ambientCoulourLocation,colour.x,colour.y,colour.z);
    glUniform1f(ambientIntensityLocation,ambientIntensity);
    glUniform3f(directionLocation,direction.x,direction.y,direction.z);
    glUniform1f(diffuseIntensityLocation,diffuseIntensity);

}


Light::~Light()
{

}