#include "Plane.h"



Plane::Plane(glm::vec3 a_position, GLuint a_size)
{
	m_vPosition = a_position;
	m_vLightPos = a_position + glm::vec3(0.0f, 5.0f, 0.0f);
	m_shaderProgram = new ShaderProgram(Shader::planeShader);
	m_vPlaneColour = glm::vec3(1.0f, 1.0f, 1.0f);
	// Plane grid
	m_uiGridSize = a_size;
	m_uiVertNum = a_size;
	m_m4Transform = Maths::createTransormationMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	m_planeModel = DynamicModels::grid(m_uiGridSize, m_uiVertNum);
	// textures
	m_diffuseTexture = new Texture("res/textures/wood.jpg");
	m_specularTexture = new Texture("res/textures/dirt.png");
	m_emissiveTexture = new Texture("res/textures/snow.png");
	
	m_fShininess = 32.0f;
}


Plane::~Plane()
{
	delete m_planeModel;
	// set ptrs to null
	m_planeModel = nullptr;
}

void Plane::draw(Camera & a_camera)
{
	float time = (float)glfwGetTime();
	// start shader
	m_shaderProgram->start();
	// pass camera position to shader 
	m_shaderProgram->uniformMat4("view", a_camera.viewMatrix());
	// pass camera projection to shader
	m_shaderProgram->uniformMat4("projection", a_camera.projection());
	// pass transform to shader
	m_shaderProgram->uniformMat4("model", m_m4Transform);
	// set lamp object colour
	m_shaderProgram->uniformVec3("lightPos", m_vLightPos);
	m_shaderProgram->uniformVec3("viewPos", a_camera.position());
	m_shaderProgram->uniformBool("blinn", 0);
	m_shaderProgram->uniformFloat("time", time);

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_diffuseTexture->ID());
	glUniform1i(glGetUniformLocation(m_shaderProgram->ID(), "texture_diffuse"), 0);

	// bind vertex array
	glBindVertexArray(m_planeModel->vaoID());
	// draw arrays
	glDrawElements(GL_TRIANGLES, m_planeModel->vertexCount(), GL_UNSIGNED_INT, 0);
	// unbind vertex array
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// stop shader
	m_shaderProgram->stop();
}


