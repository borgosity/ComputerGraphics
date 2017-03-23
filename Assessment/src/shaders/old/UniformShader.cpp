#include "UniformShader.h"

// Static Shader files
const GLchar * UNIFORM_VERTEX_FILE = "src/shaders/uniformShader.vs";
const GLchar * UNIFORM_FRAGMENT_FILE = "src/shaders/uniformShader.fs";



UniformShader::UniformShader() : ShaderProgram(UNIFORM_VERTEX_FILE, UNIFORM_FRAGMENT_FILE)
{
	// attributes bound in shader file
}


UniformShader::~UniformShader()
{
}

void UniformShader::uniform4f(const GLchar * a_uniformName, glm::vec4 & a_values)
{
	GLint uniformLocation = glGetUniformLocation(m_uiProgramID, a_uniformName);
	glUniform4f(uniformLocation, a_values.x, a_values.y, a_values.z, a_values.w);
}

void UniformShader::uniform3f(const GLchar * a_uniformName, glm::vec3 & a_values)
{
	GLint uniformLocation = glGetUniformLocation(m_uiProgramID, a_uniformName);
	glUniform3f(uniformLocation, a_values.x, a_values.y, a_values.z);
}

void UniformShader::uniform2f(const GLchar * a_uniformName, glm::vec2 & a_values)
{
	GLint uniformLocation = glGetUniformLocation(m_uiProgramID, a_uniformName);
	glUniform2f(uniformLocation, a_values.x, a_values.y);
}

void UniformShader::bindAttributes()
{
	//bindAttribute(0, "position");	// bind the 'position' variable to position 0
	//bindAttribute(1, "color");		// bind the 'color' variable to position 1
}


