#include "DirectionalLight.h"

DirectionalLight::DirectionalLight():Light(),
direction(glm::vec3(0.f,-1.f,0.f))
{

}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat di, glm::vec3 dir)
:Light(red,green,blue,aIntensity,di),direction(glm::vec3(dir.x,dir.y,dir.z))
{

}

void DirectionalLight::use(sLightLocation lightloc){
    Light::use(lightloc);
    glUniform3f(lightloc.directionLocation,direction.x,direction.y,direction.z);
}
