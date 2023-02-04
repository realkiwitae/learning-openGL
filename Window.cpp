#include "Window.h"
#include <iostream>
#include "./utils/utils.hpp"

extern double now;

Window::~Window()
{
    glfwTerminate();
}

int Window::initialise(){
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

    mainWindow = glfwCreateWindow(w,h,"Hello OpenGL", NULL, NULL);
    if(!mainWindow){
        std::cout << "GLFW window creation failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    glfwGetFramebufferSize(mainWindow,&bufferW,&bufferH);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // ha,d key + mouse;
    createCallbacks();
  //  glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK){
        std::cout << "GLFW window creation failed" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    
    glEnable(GL_DEPTH_TEST);

    // Setup viewpoortsize
    glViewport(0,0,bufferW,bufferH);

    glfwSetWindowUserPointer(mainWindow,this);

}

void Window::createCallbacks(){
    glfwSetKeyCallback(mainWindow,handleKeys);
    glfwSetCursorPosCallback(mainWindow,handleMouse);
}

void Window::handleKeys(GLFWwindow* w ,int key, int code, int action, int mode){
    Window* thew = static_cast<Window*>(glfwGetWindowUserPointer(w));
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(w,GL_TRUE);
    }

    if(key>=0 && key < 1024){
        if(action == GLFW_PRESS){
            thew->keys |= 1UL << key;
        }else if(action == GLFW_RELEASE){
            thew->keys &= ~(1UL << key);
        }
    }
}

void Window::handleMouse(GLFWwindow* w , double xpos, double ypos){
    Window* thew = static_cast<Window*>(glfwGetWindowUserPointer(w));
    if(thew->mouseFirstMoved){
        thew->lastx = xpos;
        thew->lasty = ypos;       
        thew->mouseFirstMoved = false;
    }
    thew->xchange = xpos - thew->lastx;
    thew->ychange = -ypos + thew->lasty;

    thew->lastx = xpos;
    thew->lasty = ypos;   
}
