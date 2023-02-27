#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "./gameObject/Camera.h"
#include "./utils/utils.hpp"
#include "./textures/Texture.h"
#include "./gameObject/Light.h"
#include "Material.h"

#include <ctime>
#include <iomanip>
#include <fstream>
// Win dimensions
const GLint WIDTH = 1366, HEIGHT = 768;
extern double now,last_time,delta_time;

std::vector<Mesh*> meshList = {};
std::vector<Shader> shaderList = {};
Window mainWindow;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

Material shinyMaterial;
Material dullMaterial;

Light mainlight;

// Vertex shader
static const std::string vShader = "./shaders/vertex_shader.glsl";
static const std::string fShader = "./shaders/fragment_shader.glsl"; 

void calculateAverageNormals(unsigned int* indices, unsigned int indicesCount,
     float* vertices, unsigned int verticesCount,
     unsigned int vLength, unsigned int normalOffset )
{
 	for (size_t i = 0; i < indicesCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
		
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticesCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects(){

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

    calculateAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh* obj = new Mesh();
    obj->create(vertices,indices,32,12);
    meshList.push_back(obj);
    Mesh* obj1 = new Mesh();
    obj1->create(vertices,indices,32,12);
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

    camera = Camera(glm::vec3(0.f),glm::vec3(0.f,1.f,0.f),-90.f,0.f,5.f,.5f);

    brickTexture = Texture("./textures/brick.png");
    brickTexture.load();
    dirtTexture = Texture("./textures/dirt.png");
    dirtTexture.load();

    shinyMaterial = Material(1.f, 32);
    dullMaterial = Material(.3f,4);

    mainlight = Light(1.f,1.f,1.f,.2f,
                     glm::vec3(2.f,-1.f,-2.f),.3f);

    GLuint uniformProjection = 0,uniformModel = 0,uniformView = 0,uniformEyePos = 0,
        uniformAmbientIntensity=0,uniformAmbientColour=0,
        uniformDirection = 0,uniformDiffuseIntensity = 0,
        uniformSpecularIntensity = 0, uniformShininess = 0;
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
        uniformAmbientColour = shaderList[0].getAmbientColourLocation();
        uniformAmbientIntensity = shaderList[0].getAmbientIntensityLocation();
        uniformDirection = shaderList[0].getDirectionLocation();
        uniformDiffuseIntensity = shaderList[0].getDiffuseIntensityLocation();
        uniformSpecularIntensity = shaderList[0].getSpecularIntensityLocation();
        uniformShininess = shaderList[0].getShininessLocation();
        uniformEyePos = shaderList[0].getEyePositionLocation();

        mainlight.use(uniformAmbientIntensity,uniformAmbientColour, 
                    uniformDiffuseIntensity, uniformDirection);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePos, camera.getPosition().x,camera.getPosition().y,camera.getPosition().z);

		glm::mat4 model(1.0f);	
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        // model = glm::rotate(model,glm::radians(45.f),glm::vec3(0.f,1.f,0.f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        brickTexture.use();
        shinyMaterial.use(uniformSpecularIntensity,uniformShininess);
		meshList[0]->render();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
        // model = glm::rotate(model,glm::radians(45.f),glm::vec3(0.f,1.f,0.f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.use();
        dullMaterial.use(uniformSpecularIntensity,uniformShininess);
		meshList[1]->render();

        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}
