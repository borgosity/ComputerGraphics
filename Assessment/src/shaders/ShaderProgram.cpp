#include "ShaderProgram.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


ShaderProgram::ShaderProgram()
{
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

	m_iSuccess = 0;
	m_uiVertexShaderID = loadShader(m_cpVertexShaderSource, GL_VERTEX_SHADER);
	m_uiFragmentShaderID = loadShader(m_cpFragmentShaderSource, GL_FRAGMENT_SHADER);
	linkShaders(m_uiVertexShaderID, m_uiFragmentShaderID);
}

ShaderProgram::ShaderProgram(ShaderPath a_shaderPath)
{
	// Shaders
	readFiles(a_shaderPath.vertexShader.c_str(), a_shaderPath.fragmentShader.c_str());
	m_fsName = a_shaderPath.fragmentShader;
	m_vsName = a_shaderPath.vertexShader;

	m_iSuccess = 0;
	m_uiVertexShaderID = loadShader(m_cpVertexShaderSource, GL_VERTEX_SHADER);
	m_uiFragmentShaderID = loadShader(m_cpFragmentShaderSource, GL_FRAGMENT_SHADER);
	linkShaders(m_uiVertexShaderID, m_uiFragmentShaderID);
}

ShaderProgram::~ShaderProgram()
{
}

/// start using shader program
void ShaderProgram::start()
{
	glUseProgram(m_uiProgramID);
}

/// stop using shader program
void ShaderProgram::stop()
{
	glUseProgram(0);
}

/// clean up shaders and shader program
void ShaderProgram::cleanUp()
{
	stop();
	glDetachShader(m_uiProgramID, m_uiVertexShaderID);
	glDetachShader(m_uiProgramID, m_uiFragmentShaderID);
	glDeleteShader(m_uiVertexShaderID);
	glDeleteShader(m_uiFragmentShaderID);
	glDeleteProgram(m_uiProgramID);
}

/// bind attribute to shader program
/*
	the below was commented out in favour of attribute binding in shader files
*/
//void ShaderProgram::bindAttribute(GLuint a_attribute, const GLchar * a_variableName)
//{
//	glBindAttribLocation(m_uiProgramID, a_attribute, a_variableName);
//}
void ShaderProgram::uniformVec4(const GLchar * a_uniformName, glm::vec4 & a_values)
{
	GLint uniformLocation = glGetUniformLocation(m_uiProgramID, a_uniformName);
	glUniform4f(uniformLocation, a_values.x, a_values.y, a_values.z, a_values.w);
}

void ShaderProgram::uniformVec3(const GLchar * a_uniformName, glm::vec3 & a_values)
{
	GLint uniformLocation = glGetUniformLocation(m_uiProgramID, a_uniformName);
	glUniform3f(uniformLocation, a_values.x, a_values.y, a_values.z);
}

void ShaderProgram::uniformVec2(const GLchar * a_uniformName, glm::vec2 & a_values)
{
	GLint uniformLocation = glGetUniformLocation(m_uiProgramID, a_uniformName);
	glUniform2f(uniformLocation, a_values.x, a_values.y);
}

void ShaderProgram::uniformFloat(const GLchar * a_uniformName, GLfloat a_value)
{
	GLint uniformLocation = glGetUniformLocation(m_uiProgramID, a_uniformName);
	glUniform1f(uniformLocation, a_value);
}

void ShaderProgram::uniformBool(const GLchar * a_uniformName, bool a_value)
{
	float boolValue = 0;
	GLint uniformLocation = glGetUniformLocation(m_uiProgramID, a_uniformName);
	// since there is no bool in gl shader code, we use 1 to denote true and 0 to denote false
	glUniform1f(uniformLocation, (a_value ? boolValue = 1.0f : boolValue = 0.0f));
}

void ShaderProgram::uniformInt(const GLchar * a_uniformName, GLuint a_value)
{
	GLint uniformLocation = glGetUniformLocation(m_uiProgramID, a_uniformName);
	glUniform1i(uniformLocation, a_value);
}

void ShaderProgram::uniformMat4(const GLchar * a_uniformName, glm::mat4 & a_values)
{
	GLint uniformLocation = glGetUniformLocation(m_uiProgramID, a_uniformName);
	// mat4 needs to be converted to an opengl friendly format using 'value_ptr'
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(a_values));
}


/// pull shader code from file
std::string & ShaderProgram::readFile(std::string a_filePath)
{
	
	m_spShaderSourceTemp = "";
	std::string shaderSource;
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::badbit);

	try {
		// open file
		shaderFile.open(a_filePath);
		std::stringstream shaderStream;
		// read file buffer contents into stream
		shaderStream << shaderFile.rdbuf();
		// close file handler
		shaderFile.close();
		// convert stream
		shaderSource = shaderStream.str();
	}
	catch(std::ifstream::failure e) {
		std::cout << "ERROR -> Failure reading shader file: " 
			<< "\n -- Vertex Shader = " << m_vsName
			<< "\n -- Fragment Shader = " << m_fsName
			<< std::endl;
	}
	// return shader source 
	m_spShaderSourceTemp = shaderSource;

	return m_spShaderSourceTemp;
}

void ShaderProgram::readFiles(std::string a_vsFilePath, std::string a_fsFilePath)
{
	// error check without failing
	std::ifstream file1(a_vsFilePath);
	std::ifstream file2(a_fsFilePath);
	std::string badFile;
	!file1.good() ? badFile = ("\n - " + a_vsFilePath) : badFile = "";
	!file2.good() ? badFile += ( "\n - " + a_fsFilePath) : badFile += "";
	if (badFile.length() > 0)
		std::cout << "ERROR -> SHADER read failure: " << badFile << std::endl;


	std::string vsShaderSource;
	std::ifstream vsShaderFile;
	std::string fsShaderSource;
	std::ifstream fsShaderFile;
	// allow throwing of exceptions
	vsShaderFile.exceptions(std::ifstream::badbit);
	fsShaderFile.exceptions(std::ifstream::badbit);

	try {
		// open file
		vsShaderFile.open(a_vsFilePath);
		fsShaderFile.open(a_fsFilePath);
		std::stringstream vsShaderStream;
		std::stringstream fsShaderStream;
		// read file buffer contents into stream
		vsShaderStream << vsShaderFile.rdbuf();
		fsShaderStream << fsShaderFile.rdbuf();
		// close file handler
		vsShaderFile.close();
		fsShaderFile.close();
		// convert stream
		vsShaderSource = vsShaderStream.str();
		fsShaderSource = fsShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR -> Failure reading shader file: " 
			<< "\n -- Vertex Shader = " << m_vsName
			<< "\n -- Fragment Shader = " << m_fsName
			<< std::endl;
	}
	// return shader source 
	m_cpVertexShaderSource = new GLchar[vsShaderSource.length() + 1];
	m_cpFragmentShaderSource = new GLchar[fsShaderSource.length() + 1];

	strcpy_s((char*)m_cpVertexShaderSource, vsShaderSource.length() + 1, vsShaderSource.c_str());
	strcpy_s((char*)m_cpFragmentShaderSource, fsShaderSource.length() + 1, fsShaderSource.c_str());

}

/// load shader from file
GLuint ShaderProgram::loadShader(const GLchar * a_shaderSource, GLuint a_shaderType)
{
	// Vertex shader
	GLuint shaderID = glCreateShader(a_shaderType);
	glShaderSource(shaderID, 1, &a_shaderSource, NULL);
	glCompileShader(shaderID);
	// Check for compile time errors
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &m_iSuccess);
	// log failure
	if (!m_iSuccess) {
		glGetShaderInfoLog(shaderID, 512, NULL, m_iInfoLog);
		std::cout << "ERROR -> SHADER COMPILATION_FAILED\n" 
			<< "\n -- Vertex Shader = " << m_vsName
			<< "\n -- Fragment Shader = " << m_fsName
			<< "\n" << m_iInfoLog << std::endl;
	}
	// return ID
	return shaderID;
}

/// link shaders to a shader program
void ShaderProgram::linkShaders(GLuint a_vertexShader, GLuint a_fragmentShader)
{
	// Link shaders
	m_uiProgramID = glCreateProgram();
	glAttachShader(m_uiProgramID, a_vertexShader);
	glAttachShader(m_uiProgramID, a_fragmentShader);
	// -> binding attributes would normally occur here, however, binding is done in the shader files using 'location'
	glLinkProgram(m_uiProgramID);
	// Check for linking errors
	glGetProgramiv(m_uiProgramID, GL_LINK_STATUS, &m_iSuccess);
	if (!m_iSuccess) {
		glGetProgramInfoLog(m_uiProgramID, 512, NULL, m_iInfoLog);
		std::cout << "ERROR -> SHADER PROGRAM LINKING_FAILED" 
			<< "\n -- Vertex Shader = " << m_vsName
			<< "\n -- Fragment Shader = " << m_fsName
			<< "\n" << m_iInfoLog << std::endl;
	}

}


