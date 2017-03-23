#pragma once
#include "ShaderProgram.h"


class StaticShader :
	public ShaderProgram
{
public:
	StaticShader();
	StaticShader(ShaderPath a_shaderPath);
	virtual ~StaticShader();

	void getAllUniformLocations();
	// load
	virtual void loadModelTransform(glm::mat4 a_modelTransform);
	virtual void loadCamera(Camera & a_camera);
	virtual void loadLight(Light & a_light);
	virtual void loadShineVariables(GLfloat damper, GLfloat reflectivity);
	// bind
	virtual void loadTextures(TexturedModel & a_texturedModel);



private:
	GLuint m_iLocModelTansform;
	GLuint m_iLocProjMatrix;
	GLuint m_iLocViewMatrix;
	GLuint m_iLocLightPos;
	GLuint m_iLocLightColour;
	GLuint m_iLocShineDamper;
	GLuint m_iLocReflectivity;
	// textures
	GLuint m_iLocModelTexture;
	GLuint m_iLocNormalTexture;
};

