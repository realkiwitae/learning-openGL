#include "Light.h"

Light::Light():colour(1.f,1.f,1.f),ambientIntensity(1.f)
{

}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity)
:colour(red,green,blue),ambientIntensity(aIntensity)
{

}

void Light::use(GLfloat ambientIntensityLocation, GLfloat ambientCoulourLocation){
    glUniform3f(ambientCoulourLocation,colour.x,colour.y,colour.z);
    glUniform1f(ambientIntensityLocation,ambientIntensity);

}


Light::~Light()
{

}