#include "TexturedModel.h"


TexturedModel::TexturedModel(RawModel & a_model, Texture & a_texture)
{
	m_rawModel = &a_model;
	m_textureA = &a_texture;
	m_textureB = nullptr;
	m_shineDamper = 1;
	m_reflectivity = 0;
}

TexturedModel::TexturedModel(RawModel & a_model, Texture & a_textureA, Texture & a_textureB)
{
	m_rawModel = &a_model;
	m_textureA = &a_textureA;
	m_textureB = &a_textureB;
	m_shineDamper = 1;
	m_reflectivity = 0;
}

TexturedModel::TexturedModel(RawModel & a_model, Texture & a_textureA, Texture & a_textureB, GLuint a_shaderProgramID)
{
	m_rawModel = &a_model;
	m_textureA = &a_textureA;
	m_textureB = &a_textureB;
	m_shaderProgramID = a_shaderProgramID;
	m_shineDamper = 1;
	m_reflectivity = 0;
}

TexturedModel::~TexturedModel()
{
}
/// binds textures to shader uniforms
void TexturedModel::bindTextures(const GLchar * a_uniformA, const GLchar * a_uniformB)
{
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureA->ID());
	glUniform1i(glGetUniformLocation(m_shaderProgramID, a_uniformA), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textureB->ID());
	glUniform1i(glGetUniformLocation(m_shaderProgramID, a_uniformB), 1);
}
