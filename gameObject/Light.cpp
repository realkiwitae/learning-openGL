#include "Light.h"

Light::Light():colour(1.f,1.f,1.f),ambientIntensity(1.f),diffuseIntensity(0)
{

}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat di)
:colour(red,green,blue),ambientIntensity(aIntensity),diffuseIntensity(di)
{

}

void Light::use(sLightLocation lightLoc){
    glUniform3f(lightLoc.ambientCoulourLocation,colour.x,colour.y,colour.z);
    glUniform1f(lightLoc.ambientIntensityLocation,ambientIntensity);
    glUniform1f(lightLoc.diffuseIntensityLocation,diffuseIntensity);
}


Light::~Light()
{

}