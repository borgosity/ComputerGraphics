#include "Entity.h"
#include <gtx/matrix_decompose.hpp>



Entity::Entity(TexturedModel * a_texturedModel, glm::vec3 a_position, glm::vec3 a_rotation, GLfloat a_scale)
{
	m_texturedModel = a_texturedModel;
	m_vPosition = a_position;
	m_vRotation = a_rotation;
	m_fScale = a_scale;
	m_mTransform = Maths::createTransormationMatrix(a_position, a_rotation, a_scale);
}

Entity::~Entity()
{
}
/// binds textures to shader uniforms
void Entity::bindTextures(const GLchar * a_uniformA, const GLchar * a_uniformB)
{
	m_texturedModel->bindTextures(a_uniformA, a_uniformB);
}
/// current position += vec3
void Entity::movePosition(glm::vec3 a_position)
{
	m_vPosition += a_position;
}
/// current rotation += vec3
void Entity::rotate(glm::vec3 a_rotation)
{
	m_vRotation += a_rotation;
}
/// current scale += GLfloat
void Entity::scaleSize(GLfloat a_scale)
{
	m_fScale += a_scale;
}



