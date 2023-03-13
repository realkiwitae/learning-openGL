#ifndef OMNISHADOWMAP_H
#define OMNISHADOWMAP_H

#pragma once

#include "ShadowMap.h"

class OmniShadowMap : public ShadowMap
{
public:
    OmniShadowMap();
    ~OmniShadowMap();

	bool Init(unsigned int width, unsigned int height)override;
	void Write()override;
	void Read(GLenum TextureUnit)override;
private:

};

#endif