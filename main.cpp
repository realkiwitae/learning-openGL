#include <GL/glew.h>
#include "glm/glm.hpp"
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <string>
#include <iostream>
#include <fstream> 

// Win dimensions
const GLint WIDTH = 800, HEIGHT = 600;


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

    //Loop until window closed
    while(!glfwWindowShouldClose(mainWindow)){
        // Get + Handl user input events
        glfwPollEvents();

        // Clear window
        glClearColor(0.f,0.f,0.f,1.f);// 0to1
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);
    }

    glfwTerminate();
    return 0;
}