#pragma once
#include "ShaderProgram.h"


class AnimeShader :
	public ShaderProgram
{
public:
	AnimeShader();
	AnimeShader(ShaderPath a_shaderPath);
	virtual ~AnimeShader();

	// load
	void update(Camera & camera, Light & a_light);
	void specularColour(glm::vec4 a_specularColour) { m_specularColour = a_specularColour; };
	void lightPosition(glm::vec3 a_lightPosition) { m_lightPosition = a_lightPosition; };

private:
	glm::vec4 m_specularColour;
	glm::vec3 m_lightPosition;

};

