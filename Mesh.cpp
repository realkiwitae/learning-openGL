#include "Mesh.h"

void Mesh::create(GLfloat *vertices, unsigned int* indices, unsigned int nv, unsigned int n){
    
    indexCount = nv;
    glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*n, indices, GL_STATIC_DRAW);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices[0]) * nv,vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3, GL_FLOAT , GL_FALSE , 0 , 0 );
    glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Mesh::render(){

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);

    glDrawElements(GL_TRIANGLES,indexCount,GL_UNSIGNED_INT,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

void Mesh::clear(){
    if(IBO != 0) glDeleteBuffers(1,&IBO),IBO = 0;
    if(VBO != 0) glDeleteBuffers(1,&VBO),VBO = 0;
    if(VAO != 0) glDeleteVertexArrays(1,&VAO),VAO = 0;
    indexCount = 0;
}