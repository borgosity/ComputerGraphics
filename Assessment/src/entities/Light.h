#pragma once
#include <glew.h>
#include <glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 a_position, glm::vec3 a_colour);
	// directional light constructor
	Light(glm::vec3 a_position, glm::vec3 a_colour, glm::vec3 a_direction, 
		  glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular);
	// point light constructo
	Light(glm::vec3 a_direction, glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular,
		  GLfloat a_constant, GLfloat a_linear, GLfloat a_quadratic);
	// spot light constructor
	Light(glm::vec3 a_position, glm::vec3 a_direction, glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular,
		  GLfloat a_constant, GLfloat a_linear, GLfloat a_quadratic, GLfloat a_cutOff);
	// soft spot light constructor
	Light(glm::vec3 a_position, glm::vec3 a_direction, glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular,
		  GLfloat a_constant, GLfloat a_linear, GLfloat a_quadratic, GLfloat a_cutOff, GLfloat a_outerCutOff);
	~Light();

	// getters
	glm::vec3 position()	{ return m_vPosition; };
	glm::vec3 colour()		{ return m_vColour; };
	glm::vec3 direction()	{ return m_vDirection; };
	// directional
	glm::vec3 ambient()		{ return m_vAmbient; };
	glm::vec3 diffuse()		{ return m_vDiffuse; };
	glm::vec3 specular()	{ return m_vSpecular; };
	// point
	GLfloat constant()		{ return  m_fConstant; };
	GLfloat linear()		{ return  m_fLinear; };
	GLfloat quadratic()		{ return  m_fQuadratic; };
	// spot
	GLfloat cutOff()		{ return  m_fCutOff; };
	GLfloat outerCutOff()   { return  m_fOuterCutOff; };

private:
	// directional/general
	glm::vec3 m_vPosition;
	glm::vec3 m_vColour;
	glm::vec3 m_vDirection;
	glm::vec3 m_vAmbient;
	glm::vec3 m_vDiffuse;
	glm::vec3 m_vSpecular;
	// point light
	GLfloat	  m_fConstant;
	GLfloat	  m_fLinear;
	GLfloat	  m_fQuadratic;
	// spot light
	GLfloat   m_fCutOff;
	GLfloat   m_fOuterCutOff;
};

