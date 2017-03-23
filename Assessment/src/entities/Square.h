#pragma once
#include <glew.h>
#include <glm.hpp>
// source includes
#include "ShaderProgram.h"
#include "DynamicModels.h"
#include "Texture.h"
#include "TexturedModel.h"
#include "Entity.h"

class Square
{
public:
	Square(glm::vec3 a_position);
	~Square();

	void draw(ShaderProgram & a_shaderProgram);
	glm::vec3 position() { return m_vPosition; };
	void texture(Texture * a_texture) { m_squareTexture = a_texture; };

private:
	glm::vec3		m_vPosition;
	glm::vec3		m_vSquareColour;

	Texture *		m_squareTexture = nullptr;
	TexturedModel *	m_squareTM = nullptr;
	Entity *		m_square = nullptr;


	void setupLamp();
};

