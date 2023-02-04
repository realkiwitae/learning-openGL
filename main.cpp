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
#include "./gameObject/Camera.h"
#include "./utils/utils.hpp"

// Win dimensions
const GLint WIDTH = 800, HEIGHT = 600;
extern double now,last_time,delta_time;

std::vector<Mesh*> meshList = {};
std::vector<Shader> shaderList = {};
Window mainWindow;
Camera camera;
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

    camera = Camera(glm::vec3(0.f),glm::vec3(0.f,1.f,0.f),-90.f,0.f,4.f,.3f);

    GLuint uniformProjection = 0,uniformModel = 0,uniformView = 0;
    glm::mat4 projection = glm::perspective(45.f,mainWindow.getBufferWidth()/mainWindow.getBufferHeight(),.1f,100.f);
    
    last_time = glfwGetTime();

    //Loop until window closed
    while(!mainWindow.shouldClose()){

        now = glfwGetTime();
        delta_time = now - last_time;
        last_time = now;

        // Get + Handl user input events
        glfwPollEvents();

        camera.keyControls(mainWindow.getkeys());
        camera.mouseControl(mainWindow.getxchange(),mainWindow.getychange());

        glClearColor(0.f,0.f,0.f,1.f);// 0to1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].use();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectionLocation();
        uniformView = shaderList[0].getViewLocation();

		glm::mat4 model(1.0f);	
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        // model = glm::rotate(model,glm::radians(45.f),glm::vec3(0.f,1.f,0.f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
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
