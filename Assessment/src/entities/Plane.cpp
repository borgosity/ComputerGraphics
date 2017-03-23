#include "Plane.h"



Plane::Plane(glm::vec3 a_position, GLuint a_size)
{
	m_vPosition = a_position;
	m_shaderProgram = new ShaderProgram(Shader::planeShader);
	m_vPlaneColour = glm::vec3(1.0f, 1.0f, 1.0f);
	// Plane grid
	m_uiGridSize = a_size;
	m_uiVertNum = a_size;
	m_m4Transform = Maths::createTransormationMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	m_planeModel = new RawModel();

	// textures
	m_diffuseTexture = new Texture("res/textures/wood.jpg");
	m_specularTexture = new Texture("res/textures/dirt.png");
	m_emissiveTexture = new Texture("res/textures/snow.png");
	
	m_fShininess = 32.0f;
	
	setupPlane();
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

	//m_m4Transform = Maths::createWorldTransformMatrix(m_vPosition, glm::vec3(0.0f, time * 15.0f, 0.0f), 1.0f);

	m_shaderProgram->start();
	// pass camera position to shader 
	m_shaderProgram->uniformMat4("view", a_camera.viewMatrix());
	// pass camera projection to shader
	m_shaderProgram->uniformMat4("projection", a_camera.projection());
	// pass transform to shader
	m_shaderProgram->uniformMat4("model", m_m4Transform);
	// set lamp object colour
	m_shaderProgram->uniformVec3("lightPos", glm::vec3(0.0f, 25.0f, 0.0f));
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

void Plane::setupPlane()
{

	DynamicModels::grid(*m_planeModel, m_uiGridSize, m_uiVertNum);

}

