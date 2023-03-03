#include "Material.h"

void Material::use(GLuint specularIntensityLocation, GLuint shininessLocation){
    glUniform1f(specularIntensityLocation,specularIntensity);
    glUniform1f(shininessLocation,shininess);
}