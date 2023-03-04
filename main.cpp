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

#include "./utils/Mesh.h"
#include "./shaders/Shader.h"
#include "Window.h"
#include "./gameObject/Camera.h"
#include "./utils/utils.h"
#include "./textures/Texture.h"
#include "./gameObject/DirectionalLight.h"
#include "./gameObject/PointLight.h"
#include "./gameObject/SpotLight.h"
#include "./textures/Material.h"

#include "./models/Model.h"

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
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainlight;
PointLight pointLights[MAX_N_POINT_LIGHTS];
SpotLight spotLights[MAX_N_SPOT_LIGHTS];

Model sw_emperorShuttle;

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
	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3,
	};
	GLfloat floorVertices[] = {
	//	x      y      z			u	  v			nx	  ny    nz
        -10.f,0.f,-10.f,         0.f,0.f,      0.0f, -1.f, 0.0f,
        10.f, 0.f, -10.f,       10.f, 0.f,     0.0f, -1.f, 0.0f,
        -10.f,0.f,10.f,         0.f , 10.f,    0.0f, -1.f, 0.0f,
        10.f,0.f,10.f,           10.f, 10.f,   0.0f, -1.f, 0.0f
	};

    calculateAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh* obj = new Mesh();
    obj->create(vertices,indices,32,12);
    meshList.push_back(obj);
    Mesh* obj1 = new Mesh();
    obj1->create(vertices,indices,32,12);
    meshList.push_back(obj1);
    Mesh* obj2 = new Mesh();
    obj2->create(floorVertices,floorIndices,32,6);
    meshList.push_back(obj2);
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
    brickTexture.loadAlpha();
    dirtTexture = Texture("./textures/dirt.png");
    dirtTexture.loadAlpha();
    plainTexture = Texture("./textures/plain.png");
    plainTexture.loadAlpha();

    shinyMaterial = Material(1.f, 32);
    dullMaterial = Material(.3f,4);

//models
    sw_emperorShuttle = Model();
    sw_emperorShuttle.load("Star Wars emperor shuttle");


    mainlight = DirectionalLight(1.f,1.f,1.f,
                                .1f,.1f,
                                glm::vec3(0.f,0.f,-1.f));

    unsigned int pointLightCount = 1;

    pointLights[0] = PointLight(0.f,0.f,1.f,
                            .1f,.1f,
                            glm::vec3(4.f,0.f,0.f),
                            .3f,.2f,.1f);

    // pointLights[1] = PointLight(0.f,1.f,0.f,
    //                         .1f,.1f,
    //                         glm::vec3(-4.f,2.f,0.f),
    //                         .3f,.1f,.1f);

    // pointLights[2] = PointLight(0.f,1.f,0.f,
    //                         .1f,1.f,
    //                         glm::vec3(-4.f,0.f,0.f),
    //                         .3f,.2f,.1f);

    unsigned int spotLightCount = 2;

    spotLights[0] = SpotLight(1.f,1.f,1.f,
                            .0f,2.f,glm::vec3(0.f),1.f,.0f,.0f,
                            glm::vec3(0.f,-1.f,0.f),
                            15.f
                            );
    spotLights[1] = SpotLight(1.f,1.f,1.f,
                            1.f,1.f,glm::vec3(0.f,-1.5f,0.f),1.f,.0f,.0f,
                            glm::vec3(-100.f,-1.f,0.f),
                            20.f
                            );

    GLuint uniformProjection = 0,uniformModel = 0,uniformView = 0,uniformEyePos = 0,
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
        uniformSpecularIntensity = shaderList[0].getSpecularIntensityLocation();
        uniformShininess = shaderList[0].getShininessLocation();
        uniformEyePos = shaderList[0].getEyePositionLocation();
        
        spotLights[0].setFlash(camera.getPosition() + glm::vec3(.0f,-.3f,0.f),camera.getDirection());

        shaderList[0].setDirectionalLight(&mainlight);
        shaderList[0].setPointLights(pointLights,pointLightCount);
        shaderList[0].setSpotLights(spotLights,spotLightCount);
		
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

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -0.f));
        // model = glm::rotate(model,glm::radians(45.f),glm::vec3(0.f,1.f,0.f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.use();
        shinyMaterial.use(uniformSpecularIntensity,uniformShininess);
        meshList[2]->render();
        
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -0.f));
        // model = glm::rotate(model,glm::radians(45.f),glm::vec3(0.f,1.f,0.f));
		model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        shinyMaterial.use(uniformSpecularIntensity,uniformShininess);
        sw_emperorShuttle.render();
        

        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}
