#ifndef WINDOW_H
#define WINDOW_H

#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window():w(800),h(600){}
    Window(GLuint _w, GLuint _h):w(_w),h(_h){}
    ~Window();
    int initialise();
    GLfloat getBufferWidth(){return bufferW;}
    GLfloat getBufferHeight(){return bufferH;}

    bool shouldClose(){glfwWindowShouldClose(mainWindow);}
    void swapBuffers(){glfwSwapBuffers(mainWindow);}


private:
    GLFWwindow *mainWindow;
    int bufferW,bufferH;
    GLuint w,h;
};

#endif