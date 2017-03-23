#pragma once
#include "ShaderProgram.h"


class SpotLightShader :
	public ShaderProgram
{
public:
	SpotLightShader();
	SpotLightShader(ShaderPath a_shaderPath);
	virtual ~SpotLightShader();

	// load
	void update(Camera & camera, Light & a_light);

private:

};

