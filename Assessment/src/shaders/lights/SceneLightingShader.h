#pragma once
#include "ShaderProgram.h"


class SceneLightingShader :
	public ShaderProgram
{
public:
	SceneLightingShader();
	SceneLightingShader(ShaderPath a_shaderPath);
	virtual ~SceneLightingShader();

	// load
	void update(Camera & camera, Light & a_light);

private:

};

