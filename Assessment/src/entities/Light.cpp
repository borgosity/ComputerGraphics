#include "Light.h"



Light::Light()
{
	m_vPosition = glm::vec3(0, 1, 0);
	m_vColour	= glm::vec3(1, 1, 1);
}

Light::Light(glm::vec3 a_position, glm::vec3 a_colour)
{
	m_vPosition = a_position;
	m_vColour	= a_colour;
}
/// directional light constructor
Light::Light(glm::vec3 a_position, glm::vec3 a_colour, glm::vec3 a_direction, 
			 glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular)
{
	m_vPosition		= a_position;
	m_vColour		= a_colour;
	m_vDirection	= a_direction;
	m_vAmbient		= a_ambient;
	m_vDiffuse		= a_diffuse;
	m_vSpecular		= a_specular;
}
/// point light constructor
Light::Light(glm::vec3 a_direction, glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular, 
			 GLfloat a_constant, GLfloat a_linear, GLfloat a_quadratic)
{
	m_vPosition		= -a_direction;
	m_vColour		= glm::vec3(0);
	m_vDirection	= a_direction;
	m_vAmbient		= a_ambient;
	m_vDiffuse		= a_diffuse;
	m_vSpecular		= a_specular;
	m_fConstant		= a_constant;
	m_fLinear		= a_linear;
	m_fQuadratic	= a_quadratic;
}

/// hard spot light constructor
Light::Light(glm::vec3 a_position, glm::vec3 a_direction, glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular, GLfloat a_constant, GLfloat a_linear, GLfloat a_quadratic, GLfloat a_cutOff)
{
	m_vPosition		= a_position;
	m_vColour		= glm::vec3(0);
	m_vDirection	= a_direction;
	m_vAmbient		= a_ambient;
	m_vDiffuse		= a_diffuse;
	m_vSpecular		= a_specular;
	m_fConstant		= a_constant;
	m_fLinear		= a_linear;
	m_fQuadratic	= a_quadratic;
	m_fCutOff		= a_cutOff;
}

/// soft spot light constructor
Light::Light(glm::vec3 a_position, glm::vec3 a_direction, glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular, GLfloat a_constant, GLfloat a_linear, GLfloat a_quadratic, GLfloat a_cutOff, GLfloat a_outerCutOff)
{
	m_vPosition = a_position;
	m_vColour = glm::vec3(0);
	m_vDirection = a_direction;
	m_vAmbient = a_ambient;
	m_vDiffuse = a_diffuse;
	m_vSpecular = a_specular;
	m_fConstant = a_constant;
	m_fLinear = a_linear;
	m_fQuadratic = a_quadratic;
	m_fCutOff = a_cutOff;
	m_fOuterCutOff = a_outerCutOff;
}

Light::~Light()
{
}
