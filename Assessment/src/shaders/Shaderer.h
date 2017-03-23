#pragma once
#include <iostream>
#include <glew.h>

class Shaderer
{
public:
	Shaderer();
	~Shaderer();

	GLuint buildShader();

private:
	void vertexShader();
	void fragmentShader();
	void linkShaders();
	void cleanUp();

	GLuint m_iVertexShader;
	GLuint m_iFragmentShader;
	GLuint m_iShaderProgram;
	GLint m_iSuccess;
	GLchar m_iInfoLog[512];

	// Shaders
	const GLchar* m_cpVertexShaderSource;
	const GLchar* m_cpFragmentShaderSource;
};

