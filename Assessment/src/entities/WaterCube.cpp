#include "WaterCube.h"

WaterCube::WaterCube()
{
}

WaterCube::WaterCube(glm::vec3 a_position)
{
	m_vPosition = a_position;
	m_shaderProgram = new ShaderProgram(Shader::staticShader);
	m_vColour = glm::vec3(1.0f, 1.0f, 1.0f);
	m_m4Transform = Maths::createTransormationMatrix(a_position, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	m_cubeModel = DynamicModels::cube();

	m_diffuseTexture = new Texture("res/textures/water.png");
	m_normalTexture = new Texture("res/textures/water_normal.png");
	m_specularTexture = new Texture("res/textures/water_specular.png");
	m_emissiveTexture = new Texture("res/textures/water_emissive.png");

	m_fShininess = 32.0f;
}


WaterCube::~WaterCube()
{
	delete m_cubeModel;
	m_cubeModel = nullptr;
}

void WaterCube::draw(ShaderProgram & a_shaderProgram)
{
	float time = (float)glfwGetTime();
	m_shaderProgram->uniformMat4("model", m_m4Transform);

	// diffuse
	glActiveTexture(GL_TEXTURE0); 
	glUniform1i(glGetUniformLocation(a_shaderProgram.ID(), "material.texture_diffuse1"), 0);
	glBindTexture(GL_TEXTURE_2D, m_diffuseTexture->ID());
	// specular
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(a_shaderProgram.ID(), "material.texture_specular1"), 1);
	glBindTexture(GL_TEXTURE_2D, m_diffuseTexture->ID());
	// normal
	glActiveTexture(GL_TEXTURE2);
	glUniform1i(glGetUniformLocation(a_shaderProgram.ID(), "material.texture_normal1"), 2);
	glBindTexture(GL_TEXTURE_2D, m_diffuseTexture->ID());
	// emissive
	glActiveTexture(GL_TEXTURE3);
	glUniform1i(glGetUniformLocation(a_shaderProgram.ID(), "material.texture_emissive1"), 3);
	glBindTexture(GL_TEXTURE_2D, m_diffuseTexture->ID());

	glUniform1i(glGetUniformLocation(a_shaderProgram.ID(), "hasEmissive"), 1);
	glUniform1f(glGetUniformLocation(a_shaderProgram.ID(), "material.shininess"), m_fShininess);

	// Draw mesh
	glBindVertexArray(m_cubeModel->vaoID());
	glDrawArrays(GL_TRIANGLES, 0, m_cubeModel->vertexCount());
	glBindVertexArray(0);

	// cleanup textures
	for (GLuint i = 0; i < 4; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

