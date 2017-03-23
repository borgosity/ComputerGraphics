#pragma once
// std includes
#include <algorithm>
#include <iterator>
// opengl includes
#include <glew.h>
#include <glm.hpp>
#include <gtc\noise.hpp>
// source includes
#include "ShaderProgram.h"
#include "DynamicModels.h"
#include "Texture.h"
#include "TexturedModel.h"
#include "Entity.h"

class Plane
{
public:
	Plane(glm::vec3 a_position, GLuint a_size);
	~Plane();

	void draw(Camera & a_camera);
	glm::vec3 position() { return m_vPosition; };
	glm::vec3 position(glm::vec3 a_position) { m_vPosition = a_position; return m_vPosition; };
	glm::mat4 transform() { return m_m4Transform; };
	glm::mat4 transform(glm::mat4 a_transform) { m_m4Transform = a_transform; return m_m4Transform; };

private:
	glm::vec3		m_vPosition;
	glm::mat4		m_m4Transform;
	glm::vec3		m_vPlaneColour;
	GLuint			m_uiSize;

	// default grid settings
	GLuint			m_uiGridSize;
	GLuint			m_uiVertNum;
	
	RawModel *		m_planeModel = nullptr;

	GLfloat			m_fShininess;

	ShaderProgram * m_shaderProgram = nullptr;
	
	Texture *		m_diffuseTexture = nullptr;
	Texture *		m_specularTexture = nullptr;
	Texture *		m_emissiveTexture = nullptr;

	TexturedModel *	m_planeTM = nullptr;
	Entity *		m_plane = nullptr;
	
	void setupPlane();
};

