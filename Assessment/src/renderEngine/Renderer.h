#pragma once
#include <glew.h>

#include "RawModel.h"
#include "TexturedModel.h"
#include "ShaderProgram.h"
#include "Entity.h"
#include "Maths.h"
#include "ConstValues.h"
#include "Camera.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	//void createProjectionMatrix();
	void prepare(GLfloat red, GLfloat green, GLfloat blue);
	void prepare(glm::vec4 a_colour);
	void render(RawModel * model);
	void render(RawModel * model, GLuint shaderProgram);
	void renderInd(RawModel * model);
	void renderTexturedModel(TexturedModel * a_texturedModel);
	void renderEntity(Entity * a_entity, ShaderProgram * a_shader);

	void renderWithShader(Entity * a_entity, ShaderProgram * a_shader);

	void bindRenderTexture(TexturedModel * a_texturedModel);
	void bindTexture(TexturedModel * a_texturedModel, ShaderProgram * a_shader, const char * a_uniformName, int a_texNumber);
};

