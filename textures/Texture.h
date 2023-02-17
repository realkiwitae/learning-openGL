#ifndef TEXTURE_H
#define TEXTURE_H

#pragma once

#include <GL/glew.h>
#include "../utils/stb_image.h"

#include <string>

class Texture
{
public:
    Texture();
    Texture(std::string f);
    ~Texture();

    void load();
    void use();
    void clear();

private:
    GLuint id;
    int w,h,bitdepth;
    std::string filelocation;
};

#endif