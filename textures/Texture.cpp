#include "Texture.h"
#include <iostream>

Texture::Texture() : id(0),w(0),h(0),bitdepth(0),filelocation("")
{

}

Texture::Texture(std::string f) : id(0),w(0),h(0),bitdepth(0),filelocation(f)
{

}

Texture::~Texture()
{
    clear();
}

bool Texture::load(){
    unsigned char*texData = stbi_load(filelocation.c_str(), &w, &h , &bitdepth,0);
    if(!texData){
        std::cerr << "Failed to find at " << filelocation << std::endl;
        return false;
    }
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texData);
    return true;
}

bool Texture::loadAlpha(){
    unsigned char*texData = stbi_load(filelocation.c_str(), &w, &h , &bitdepth,0);
    if(!texData){
        std::cerr << "Failed to find at " << filelocation << std::endl;
        return false;
    }
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texData);
    return true;
}

void Texture::use(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::clear(){
    glDeleteTextures(1, &id);
    id = 0;
    h=0;
    w=0;
    bitdepth = 0;
    filelocation = "";
}