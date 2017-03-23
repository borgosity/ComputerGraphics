#pragma once
#include <iostream>
#include <glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include "Shaders.h"
#include "TexturedModel.h"
#include "Light.h"
#include "Camera.h"

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(ShaderPath a_shaderPath);
	virtual ~ShaderProgram();
	// shader control
	void start();
	void stop();
	void cleanUp();
	// uniform functions
	void uniformBool(const GLchar * a_uniformName, bool a_value);
	void uniformInt(const GLchar * a_uniformName, GLuint a_value);
	void uniformFloat(const GLchar * a_uniformName, GLfloat a_value);
	void uniformVec2(const GLchar * a_uniformName, glm::vec2 & a_values);
	void uniformVec3(const GLchar * a_uniformName, glm::vec3 & a_values);
	void uniformVec4(const GLchar * a_uniformName, glm::vec4 & a_values);
	void uniformMat4(const GLchar * a_uniformName, glm::mat4 & a_values);
	// load functions
	virtual void loadModelTransform(glm::mat4 a_modelTransform) {};
	virtual void loadCamera(Camera & a_camera) {};
	virtual void loadLight(Light & a_light) {};
	virtual void loadTextures(TexturedModel & a_texturedModel) {};
	virtual void loadShineVariables(GLfloat damper, GLfloat reflectivity) {};
	// variable accessors
	GLuint ID() { return m_uiProgramID; };

	/*
		the below was commented out in favour of attribute binding in shader files
	*/
	// this function must be implemented by the derived class
	// virtual void bindAttributes() = 0;
	// void bindAttribute(GLuint a_attribute, const GLchar * a_variableName);
	
protected:
	GLuint m_uiProgramID;
	GLuint m_uiVertexShaderID;
	GLuint m_uiFragmentShaderID;
	GLint m_iSuccess;
	GLchar m_iInfoLog[512];

	// Shaders
	std::string m_spShaderSourceTemp;
	const GLchar * m_cpVertexShaderSource;
	const GLchar * m_cpFragmentShaderSource;

	std::string m_vsName;
	std::string m_fsName;

	// private functions
	std::string & readFile(std::string a_filePath);
	void readFiles(std::string a_vsFilePath, std::string a_fsFilePath);
	GLuint loadShader(const GLchar * a_shaderSource, GLuint a_shaderType);
	void linkShaders(GLuint a_vertexShader, GLuint a_fragmentShader);
	// virtual functions

};

