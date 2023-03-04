#include "SpotLight.h"

SpotLight::SpotLight():PointLight(),direction(glm::vec3(0.f,-1.f,0.f)),edge(0.f),procEdge(0.f)
{

}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat di,
    glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat exp,
        glm::vec3 dir, GLfloat edg 
):PointLight( red,  green,  blue,  aIntensity,  di,
         pos,  con,  lin,  exp
), direction(glm::normalize(dir)),edge(edg),procEdge(cosf(glm::radians(edge)))
{}

void SpotLight::use(sLightLocation lightloc){
    PointLight::use(lightloc);
    glUniform3f(lightloc.directionLocation,direction.x,direction.y,direction.z);
    glUniform1f(lightloc.edgeLocation,procEdge);

}

void SpotLight::setFlash(glm::vec3 pos, glm::vec3 dir){
    position = pos;
    direction = dir;
}