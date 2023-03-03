#include "PointLight.h"

PointLight::PointLight():Light(),position(glm::vec3()),constant(1.f),linear(0.f),exponent(0.f)
{

}
PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat di,
        glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat exp
):Light(red,green,blue, aIntensity,di),position(pos),constant(con),linear(lin),exponent(exp)
{}

void PointLight::use(sLightLocation lightloc){
    Light::use(lightloc);
    glUniform3f(lightloc.positionLocation,position.x,position.y,position.z);
    glUniform1f(lightloc.constantLocation,constant);
    glUniform1f(lightloc.linearLocation,linear);
    glUniform1f(lightloc.exponentialLocation,exponent);

}