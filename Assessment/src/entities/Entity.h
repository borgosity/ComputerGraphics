#pragma once
#include <glm.hpp>
#include "TexturedModel.h"
#include "Maths.h"

class Entity
{
public:
	Entity(TexturedModel * a_texturedModel, glm::vec3 a_position, glm::vec3 a_rotation, GLfloat a_scale);
	~Entity();

	void bindTextures(const GLchar * a_uniformA, const GLchar * a_uniformB);
	void movePosition(glm::vec3 a_position);
	void rotate(glm::vec3 a_rotation);
	void scaleSize(GLfloat a_scale);

	// getters and setters
	glm::mat4 transform() { return m_mTransform; };
	glm::mat4 transform(glm::mat4 a_transform) { m_mTransform = a_transform; return m_mTransform; };
	TexturedModel * model() { return m_texturedModel; }
	TexturedModel * model(TexturedModel * a_model) { m_texturedModel = a_model; return m_texturedModel; }
	glm::vec3 *		position() { return &m_vPosition; }
	glm::vec3 *		position(glm::vec3 a_position) { m_vPosition = a_position; return &m_vPosition; }
	glm::vec3 *		rotation() { return &m_vRotation; }
	glm::vec3 *		rotation(glm::vec3 a_rotation) { m_vRotation = a_rotation;  return &m_vRotation; }
	GLfloat			scale() { return m_fScale; }
	GLfloat			scale(GLfloat a_scale) { m_fScale = a_scale; return m_fScale; }

private:
	TexturedModel * m_texturedModel;
	glm::mat4		m_mTransform;
	glm::vec3		m_vPosition;
	glm::vec3		m_vRotation;
	GLfloat			m_fScale;
	
};

