#include "Square.h"



Square::Square(glm::vec3 a_position)
{
	m_vPosition = a_position;
	setupLamp();
}


Square::~Square()
{

	delete m_squareTexture;
	delete m_squareTM;
	delete m_square;
	// set ptrs to null
	m_squareTexture = nullptr;
	m_squareTM = nullptr;
	m_square = nullptr;
}

void Square::draw(ShaderProgram & a_shaderProgram)
{
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_squareTexture->ID());
	glUniform1i(glGetUniformLocation(a_shaderProgram.ID(), "texture_diffuse1"), 0);
	// bind vertex array
	glBindVertexArray(m_squareTM->vaoID());
	// draw arrays
	glDrawArrays(GL_TRIANGLES, 0, m_squareTM->vertexCount());
	// unbind vertex array
	glBindVertexArray(0);
}

void Square::setupLamp()
{
	m_squareTexture = new Texture("res/textures/container.jpg");
	m_squareTM = new TexturedModel(*DynamicModels::square(1.0f), *m_squareTexture, *m_squareTexture, 0);
	m_square = new Entity(m_squareTM, m_vPosition, glm::vec3(90.0F, 0.0f, 0.0f), 1.0f);
}
