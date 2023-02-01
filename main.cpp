#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

// Win dimensions
const GLint WIDTH = 800, HEIGHT = 600;

std::vector<Mesh*> meshList = {};
std::vector<Shader> shaderList = {};
Window mainWindow;

// Vertex shader
static const std::string vShader = "./shaders/vertex_shader.glsl";
static const std::string fShader = "./shaders/fragment_shader.glsl"; 

void CreateObjects(){

    unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2 
    };

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
        0.f,-1.f,1.f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

    Mesh* obj = new Mesh();
    obj->create(vertices,indices,12,12);
    meshList.push_back(obj);
    Mesh* obj1 = new Mesh();
    obj1->create(vertices,indices,12,12);
    meshList.push_back(obj1);
}

void CreateShaders(){
    Shader * s = new Shader();
    s->createFromFiles(vShader,fShader);
    shaderList.push_back(*s);
}

int main(void)
{
    mainWindow = Window(WIDTH,HEIGHT);
    mainWindow.initialise();
    CreateObjects();
    CreateShaders();

    GLuint uniformProjection = 0,uniformModel = 0;
    glm::mat4 projection = glm::perspective(45.f,mainWindow.getBufferWidth()/mainWindow.getBufferHeight(),.1f,100.f);

    //Loop until window closed
    while(!mainWindow.shouldClose()){
        // Get + Handl user input events
        glfwPollEvents();

        glClearColor(0.f,0.f,0.f,1.f);// 0to1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].use();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectionLocation();

		glm::mat4 model(1.0f);	
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        // model = glm::rotate(model,glm::radians(45.f),glm::vec3(0.f,1.f,0.f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->render();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
        // model = glm::rotate(model,glm::radians(45.f),glm::vec3(0.f,1.f,0.f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->render();

        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}