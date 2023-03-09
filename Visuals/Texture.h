#pragma once

#include <GL/glew.h>

#include "../Tools/stb_image.h"

#include <string>

class Texture
{
public:
	Texture();
	Texture(std::string fileLoc);

	bool loadRGB();
	bool loadRGBA();

	void use();
	void clear();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	std::string fileLocation;
};

