#pragma once
#include "ShaderProgram.h"


class ParticleShader :
	public ShaderProgram
{
public:
	ParticleShader();
	ParticleShader(ShaderPath a_shaderPath);
	virtual ~ParticleShader();

	// load
	void update(Camera & camera, Light & a_light);

private:

};

