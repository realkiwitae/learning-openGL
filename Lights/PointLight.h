#pragma once
#include "Light.h"
#include <vector>

class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(
		GLuint shadowWidth, GLuint shadowHeight,
		GLfloat near, GLfloat far,
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	std::vector<glm::mat4> CalculateLightTransform();
	GLfloat getFarPlane(){return farPlane;}
	glm::vec3 getPosition(){return position;}
	~PointLight();

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent, farPlane;
};

