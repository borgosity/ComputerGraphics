#pragma once
#include "RawModel.h"
#include "Texture.h"

class TexturedModel
{
public:
	TexturedModel(RawModel & a_model, Texture & a_texture);
	TexturedModel(RawModel & a_model, Texture & a_textureA, Texture & a_textureB);
	TexturedModel(RawModel & a_model, Texture & a_textureA, Texture & a_textureB, GLuint a_shaderProgramID);
	~TexturedModel();

	RawModel & rawModel() { return *m_rawModel; }
	Texture & texture(unsigned int a_value) { return (a_value == 0 ? *m_textureA : *m_textureB); }
	// access raw model values
	GLuint vaoID() { return m_rawModel->vaoID(); }
	GLuint vertexCount() { return m_rawModel->vertexCount(); };
	// bind textures to shader uniforms
	void bindTextures(const GLchar * a_uniformA, const GLchar * a_uniformB);
	// does the model use indices
	bool hasIndices() { return rawModel().hasIndecies(); };
	// getters, setters
	GLfloat shine() { return m_shineDamper; };
	GLfloat shine(GLfloat a_value) { m_shineDamper = a_value; return m_shineDamper; };
	GLfloat reflection() { return m_reflectivity; };
	GLfloat reflection(GLfloat a_value) { m_reflectivity = a_value; return m_reflectivity; };

private:
	RawModel *	m_rawModel = nullptr;
	Texture *	m_textureA = nullptr;
	Texture *	m_textureB = nullptr;
	GLuint		m_shaderProgramID;
	GLfloat		m_shineDamper;
	GLfloat		m_reflectivity;

	// private functions
};

