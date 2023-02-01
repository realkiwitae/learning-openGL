#ifndef WINDOW_H
#define WINDOW_H

#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window():w(800),h(600),keys(0),xchange(0),ychange(0),mouseFirstMoved(true){}
    Window(GLuint _w, GLuint _h):w(_w),h(_h),keys(0),xchange(0),ychange(0),mouseFirstMoved(true){}
    ~Window();
    int initialise();
    GLfloat getBufferWidth(){return bufferW;}
    GLfloat getBufferHeight(){return bufferH;}

    bool shouldClose(){glfwWindowShouldClose(mainWindow);}
    void swapBuffers(){glfwSwapBuffers(mainWindow);}
    long* getkeys(){return &keys;}
    GLfloat getxchange(){GLfloat x=xchange,xchange=0;return x;}
    GLfloat getychange(){GLfloat y=ychange,ychange=0;return y;}

private:
    GLFWwindow *mainWindow;
    int bufferW,bufferH;
    GLuint w,h;
    
    long keys;
    GLfloat lastx, lasty,xchange,ychange;
    bool mouseFirstMoved;

    void createCallbacks();
    static void handleKeys(GLFWwindow* w ,int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow* w , double xpos, double ypos);
};

#endif