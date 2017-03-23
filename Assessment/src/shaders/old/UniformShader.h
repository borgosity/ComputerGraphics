#pragma once
#include "ShaderProgram.h"



class UniformShader :
	public ShaderProgram
{
public:
	UniformShader();
	virtual ~UniformShader();

	virtual void bindAttributes();
	// uniform functions
	void uniform4f(const GLchar* a_uniformName, glm::vec4 & a_values);
	void uniform3f(const GLchar * a_uniformName, glm::vec3 & a_values);
	void uniform2f(const GLchar * a_uniformName, glm::vec2 & a_values);

private:

};

