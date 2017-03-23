#pragma once
#include "ShaderProgram.h"


class LightShader :
	public ShaderProgram
{
public:
	LightShader();
	LightShader(ShaderPath a_shaderPath);
	virtual ~LightShader();

	// load
	void update(Camera & camera, Light & a_light);

private:

};

