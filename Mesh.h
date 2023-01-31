#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

class Mesh
{
public:
    Mesh():VAO(0),IBO(0),VBO(0),indexCount(0){}
    ~Mesh(){};

    void create(GLfloat *vertices, unsigned int* indices, unsigned int nv, unsigned int n);
    void render();
    void clear();

private:
    GLuint VAO,IBO, VBO;
    GLsizei indexCount;
};

#endif