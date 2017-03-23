#include "Shaderer.h"



Shaderer::Shaderer()
{
	m_iVertexShader = 0;
	m_iFragmentShader = 0;
	m_iShaderProgram = 0;
	m_iSuccess = 0;
	// Shaders
	m_cpVertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"}\0";
	m_cpFragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
}


Shaderer::~Shaderer()
{
}

GLuint Shaderer::buildShader()
{
	vertexShader();
	fragmentShader();
	linkShaders();
	cleanUp();
	return m_iShaderProgram;
}

void Shaderer::vertexShader()
{
	// Vertex shader
	m_iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_iVertexShader, 1, &m_cpVertexShaderSource, NULL);
	glCompileShader(m_iVertexShader);
	// Check for compile time errors
	glGetShaderiv(m_iVertexShader, GL_COMPILE_STATUS, &m_iSuccess);
	if (!m_iSuccess)
	{
		glGetShaderInfoLog(m_iVertexShader, 512, NULL, m_iInfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << m_iInfoLog << std::endl;
	}
}

void Shaderer::fragmentShader()
{
	// Fragment shader
	m_iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_iFragmentShader, 1, &m_cpFragmentShaderSource, NULL);
	glCompileShader(m_iFragmentShader);
	// Check for compile time errors
	glGetShaderiv(m_iFragmentShader, GL_COMPILE_STATUS, &m_iSuccess);
	if (!m_iSuccess)
	{
		glGetShaderInfoLog(m_iFragmentShader, 512, NULL, m_iInfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << m_iInfoLog << std::endl;
	}
}

void Shaderer::linkShaders()
{
	// Link shaders
	m_iShaderProgram = glCreateProgram();
	glAttachShader(m_iShaderProgram, m_iVertexShader);
	glAttachShader(m_iShaderProgram, m_iFragmentShader);
	glLinkProgram(m_iShaderProgram);
	// Check for linking errors
	glGetProgramiv(m_iShaderProgram, GL_LINK_STATUS, &m_iSuccess);
	if (!m_iSuccess) {
		glGetProgramInfoLog(m_iShaderProgram, 512, NULL, m_iInfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << m_iInfoLog << std::endl;
	}
}

void Shaderer::cleanUp()
{
	glDeleteShader(m_iVertexShader);
	glDeleteShader(m_iFragmentShader);
}



