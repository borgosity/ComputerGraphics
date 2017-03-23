#include "Renderer.h"



Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

//void Renderer::createProjectionMatrix()
//{
//	// set the cameras - FOV, Screen Ratio, near plane, far plane
//	m_mCameraProjection = glm::perspective(FOV, (GLfloat)SCREEN_W / (GLfloat)SCREEN_H, NEAR_PLANE, FAR_PLANE);
//}

void Renderer::prepare(GLfloat red, GLfloat green, GLfloat blue)
{
	// Clear the colorbuffer
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::prepare(glm::vec4 a_colour)
{
	// Clear the colorbuffer
	glClearColor(a_colour.r, a_colour.g, a_colour.b, a_colour.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/// render model
void Renderer::render(RawModel * model)
{
	// Draw model
	glBindVertexArray(model->vaoID());
	glDrawArrays(GL_TRIANGLES, 0, model->vertexCount());
	glBindVertexArray(0);
}

/// render model using specific shader program
void Renderer::render(RawModel * model, GLuint shaderProgram)
{
	// check entity isn't null
	if (model == nullptr) {
		std::cout << "\n ERROR --> RENDERING raw model was null" << std::endl;
		return;
	}
	// Draw model
	glUseProgram(shaderProgram);
	glBindVertexArray(model->vaoID());
	glDrawArrays(GL_TRIANGLES, 0, model->vertexCount());
	glBindVertexArray(0);
}

/// render model with indicies
void Renderer::renderInd(RawModel * model)
{
	// check entity isn't null
	if (model == nullptr) {
		std::cout << "\n ERROR --> RENDERING raw model was null" << std::endl;
		return;
	}
	// Draw model
	glBindVertexArray(model->vaoID());
	glDrawElements(GL_TRIANGLES, model->vertexCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::bindRenderTexture(TexturedModel * a_texturedModel)
{
	// Bind Texture
	glBindTexture(GL_TEXTURE_2D, a_texturedModel->rawModel().vaoID());
	glBindVertexArray(a_texturedModel->rawModel().vaoID());
	glDrawElements(GL_TRIANGLES, a_texturedModel->rawModel().vertexCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::bindTexture(TexturedModel * a_texturedModel, ShaderProgram * a_shader, const char * a_uniformName, int a_texNumber )
{
	glBindTexture(GL_TEXTURE_2D, a_texturedModel->rawModel().vaoID());
	glUniform1i(glGetUniformLocation(a_shader->ID(), a_uniformName), a_texNumber);
}

void Renderer::renderTexturedModel(TexturedModel * a_texturedModel)
{
	// check entity isn't null
	if (a_texturedModel == nullptr) {
		std::cout << "\n ERROR --> RENDERING textured model was null" << std::endl;
		return;
	}
	glBindVertexArray(a_texturedModel->vaoID());
	// check if the model uses indices
	if (a_texturedModel->rawModel().hasIndecies()){
		glDrawElements(GL_TRIANGLES, a_texturedModel->vertexCount(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, a_texturedModel->vertexCount());
	}

	glBindVertexArray(0);
}

void Renderer::renderEntity(Entity * a_entity, ShaderProgram * a_shader)
{
	// check entity isn't null
	if (a_entity == nullptr) {
		std::cout << "\n ERROR --> RENDERING entity was null" << std::endl;
		return;
	}
	// check shader isn't null
	if (a_shader == nullptr) {
		std::cout << "\n ERROR --> RENDERING; shader was null" << std::endl;
		return;
	}
	
	a_entity->bindTextures("diffuse", "normal");
	glBindVertexArray(a_entity->model()->vaoID());
	a_shader->uniformMat4("model", a_entity->transform());
	// check if the entities model uses indices
	if (a_entity->model()->hasIndices()) {
		glDrawElements(GL_TRIANGLES, a_entity->model()->vertexCount(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, a_entity->model()->vertexCount());
	}

	glBindVertexArray(0);
}

void Renderer::renderWithShader(Entity * a_entity, ShaderProgram * a_shader)
{
	// check entity isn't null
	if (a_entity == nullptr) {
		std::cout << "\n ERROR --> RENDERING; entity was null" << std::endl;
		return;
	}
	// check shader isn't null
	if (a_shader == nullptr) {
		std::cout << "\n ERROR --> RENDERING; shader was null" << std::endl;
		return;
	}
	// load shader variables
	a_shader->loadTextures(*a_entity->model());
	// bind entity model
	glBindVertexArray(a_entity->model()->vaoID());
	a_shader->loadModelTransform(a_entity->transform());
	a_shader->loadShineVariables(a_entity->model()->shine(), a_entity->model()->reflection());

	// check if the entities model uses indices
	if (a_entity->model()->hasIndices()) {
		glDrawElements(GL_TRIANGLES, a_entity->model()->vertexCount(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, a_entity->model()->vertexCount());
	}

	glBindVertexArray(0);
}
