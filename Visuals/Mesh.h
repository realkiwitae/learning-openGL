#pragma once

#include <GL/glew.h>

class Mesh
{
public:
	Mesh();

	void create(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void render();
	void clear();

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

