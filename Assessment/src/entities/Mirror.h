#pragma once
// std includes
#include <iostream>

// opengl includes
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// source includes
#include "RawModel.h"
#include "ConstValues.h"
#include "ShaderProgram.h"
#include "DynamicModels.h"
#include "AABB.h"

class Mirror
{
public:
	Mirror();
	Mirror(glm::vec3 a_position);
	~Mirror();

	void bindFBO();
	void unbindFBO();
	void draw(Camera & a_camera);
	glm::vec3 position() { return m_vPosition; };
	glm::vec3 position(glm::vec3 a_position) { m_vPosition = a_position; return m_vPosition; };
	glm::mat4 transform() { return m_m4Transform; };
	glm::mat4 transform(glm::mat4 a_transform) { m_m4Transform = a_transform; return m_m4Transform; };
	AABB * aabb() { return m_mirrorModel->aabb(); };

private:
	GLuint m_FBO; // frame buffer
	GLuint m_TBO; // texture buffer
	GLuint m_RBO; // render buffer

	glm::vec3		m_vPosition;
	glm::mat4		m_m4Transform;
	glm::vec3		m_vColour;
	RawModel * m_mirrorModel = nullptr;
	ShaderProgram * m_shaderProgram = nullptr;
	GLfloat			m_fShininess;

	Texture * m_testTexture = nullptr;

	void setupFBO();
	GLuint generateTexture(GLboolean a_depth, GLboolean a_stencil);
};

