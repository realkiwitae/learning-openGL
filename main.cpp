#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream> 

// Win dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader, uniformModel;

bool direction = true;
float triOffset = 0.f;
float triMaxOffset = .7f;
float triInc = .005f;
float rota = 0.f;
float curs = .4f;

// Vertex shader
static const std::string vShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
layout (location = 0) in vec3 pos;											  \n\
uniform mat4 model;											  \n\
                                                                     \n\
void main()                                                                   \n\
{                                                                             \n\
    gl_Position = model*vec4(pos,1.);				  \n\
}";

// Fragment Shader
static const std::string fShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
out vec4 colour;                                                               \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    colour = vec4(1.0, 0.0, 0.0, 1.0);                                         \n\
}";


void CreateTriangle(){

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3, GL_FLOAT , GL_FALSE , 0 , 0 );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindVertexArray(0);

}

void AddShader( GLuint theProgram,const std::string shaderCode, GLenum shaderType){
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode.c_str();
    
    GLint codeLength[1];
    codeLength[0] = shaderCode.size();

    glShaderSource(theShader , 1 , theCode, codeLength);
    glCompileShader(theShader);

    GLint res = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &res);
    if(!res){
        glGetShaderInfoLog(theShader,sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n",shaderType, eLog);
        return;
    }
    
    glAttachShader(theProgram,theShader);
}

void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader) 
	{
		printf("Failed to create shader\n");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) 
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) 
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

    uniformModel = glGetUniformLocation(shader,"model");
}
int main(void)
{
    // Initalise GLSW
    if(!glfwInit()){
        std::cout << "GLFW initialisation failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    // setup GLFW properties
    //Opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile = no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH,HEIGHT,"Hello OpenGL", NULL, NULL);
    if(!mainWindow){
        std::cout << "GLFW window creation failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow,&bufferWidth,&bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK){
        std::cout << "GLFW window creation failed" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    
    // Setup viewpoortsize
    glViewport(0,0,bufferWidth,bufferHeight);

    CreateTriangle();
    CompileShaders();

    //Loop until window closed
    while(!glfwWindowShouldClose(mainWindow)){
        // Get + Handl user input events
        glfwPollEvents();
        
        triOffset += (direction?1:-1)*triInc;
        if(fabs(triOffset)> triMaxOffset)direction=!direction;

        // Clear window
        glClearColor(0.f,0.f,0.f,1.f);// 0to1
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        rota +=.1f;
        if(rota>=360.f)rota-=360.f;

        curs += (direction?-1:1)*triInc;
        curs = glm::clamp(curs,.4f,1.f);


        glm::mat4 model(1.0f);
        model = glm::translate(model,glm::vec3(triOffset,0.f,0.f));
        model = glm::rotate(model,glm::radians(rota),glm::vec3(0.f,0.f,1.f));
        model = glm::scale(model,glm::vec3(curs,curs,1.f));

        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);

        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    glfwTerminate();
    return 0;
}