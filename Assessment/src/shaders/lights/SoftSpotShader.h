#pragma once
#include "ShaderProgram.h"


class SoftSpotShader :
	public ShaderProgram
{
public:
	SoftSpotShader();
	SoftSpotShader(ShaderPath a_shaderPath);
	virtual ~SoftSpotShader();

	// load
	void update(Camera & camera, Light & a_light);

private:

};

