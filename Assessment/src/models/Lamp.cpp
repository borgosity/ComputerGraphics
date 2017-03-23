#include "Lamp.h"



Lamp::Lamp(glm::vec3 a_position)
{
	m_vPosition = a_position;
	m_shaderProgram = new ShaderProgram(Shader::lampShader);
	setupLamp();
}


Lamp::~Lamp()
{
	delete m_shaderProgram;
	delete m_lampTexture;
	delete m_lampTM;
	delete m_lamp;
	// set ptrs to null
	m_shaderProgram = nullptr;
	m_lampTexture = nullptr;
	m_lampTM = nullptr;
	m_lamp = nullptr;
}

void Lamp::draw(Camera & a_camera)
{
	m_shaderProgram->start();
	// pass camera position to shader 
	m_shaderProgram->uniformMat4("view", a_camera.viewMatrix());
	// pass camera projection to shader
	m_shaderProgram->uniformMat4("projection", a_camera.projection());
	// pass transform to shader
	m_shaderProgram->uniformMat4("model", m_lamp->transform());
	// set lamp object colour
	m_shaderProgram->uniformVec3("lampColor", m_vLampColour);
	// set lamp loight output colour
	//m_shaderProgram->uniformVec3("lightColor", m_vLightColour);

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_lampTexture->ID());
	glUniform1i(glGetUniformLocation(m_shaderProgram->ID(), "texture_diffuse1"), 0);
	// bind vertex array
	glBindVertexArray(m_lampTM->vaoID());
	// draw arrays
	glDrawArrays(GL_TRIANGLES, 0, m_lampTM->vertexCount());
	// unbind vertex array
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	// stop shader
	m_shaderProgram->stop();
}

void Lamp::setupLamp()
{
	m_lampTexture = new Texture("res/textures/lamp.png");
	m_lampTM = new TexturedModel(*DynamicModels::cube(), *m_lampTexture, *m_lampTexture, m_shaderProgram->ID());
	m_lamp = new Entity(m_lampTM, m_vPosition, glm::vec3(0.0F, 0, 0.0f), 0.25f);
	m_vLampColour = glm::vec3(1.0f, 1.0f, 1.0f);	// set lamp object colour to white
	//m_vLightColour = glm::vec3(1.0f, 1.0f, 1.0f);	// set light's color (white)
}
