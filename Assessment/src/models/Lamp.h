#pragma once
#include <glew.h>
#include <glm.hpp>
// source includes
#include "ShaderProgram.h"
#include "DynamicModels.h"
#include "Texture.h"
#include "TexturedModel.h"
#include "Entity.h"

class Lamp
{
public:
	Lamp(glm::vec3 a_position);
	~Lamp();

	void draw(Camera & a_camera);
	glm::vec3 position() { return m_vPosition; };

private:
	glm::vec3		m_vPosition;
	glm::vec3		m_vLampColour;
	glm::vec3		m_vLightColour;

	ShaderProgram * m_shaderProgram = nullptr;
	Texture *		m_lampTexture = nullptr;
	TexturedModel *	m_lampTM = nullptr;
	Entity *		m_lamp = nullptr;


	void setupLamp();
};

