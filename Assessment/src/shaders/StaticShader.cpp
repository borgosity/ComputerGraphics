#include "StaticShader.h"



StaticShader::StaticShader()
{
}

StaticShader::StaticShader(ShaderPath a_shaderPath) : ShaderProgram(a_shaderPath)
{
	m_iLocModelTansform = 0;
	m_iLocProjMatrix = 0;
	m_iLocViewMatrix = 0;
	m_iLocLightPos = 0;
	m_iLocLightColour = 0;
	m_iLocShineDamper = 0;
	m_iLocReflectivity = 0;
	m_iLocModelTexture = 0;
	m_iLocNormalTexture = 0;
	// retrieve all the uniform locations from the shader
	getAllUniformLocations();
}


StaticShader::~StaticShader()
{
}
///
/// Get all the unifrom locations from the shaders
///
void StaticShader::getAllUniformLocations()
{
	// matrices
	m_iLocModelTansform = glGetUniformLocation(m_uiProgramID, "model");
	m_iLocProjMatrix = glGetUniformLocation(m_uiProgramID, "projection");
	m_iLocViewMatrix = glGetUniformLocation(m_uiProgramID, "view");
	// light
	m_iLocLightPos = glGetUniformLocation(m_uiProgramID, "lightPos");
	m_iLocLightColour = glGetUniformLocation(m_uiProgramID, "lightColour");
	// textures
	m_iLocModelTexture = glGetUniformLocation(m_uiProgramID, "modelTexture");
	m_iLocNormalTexture = glGetUniformLocation(m_uiProgramID, "normalTex");
	// specular
	m_iLocShineDamper = glGetUniformLocation(m_uiProgramID, "shineDamper");
	m_iLocReflectivity = glGetUniformLocation(m_uiProgramID, "reflectivity");
}

///
/// Load specular variables to the shader uniform locations
///
void StaticShader::loadShineVariables(GLfloat damper, GLfloat reflectivity)
{
	glUniform1f(m_iLocShineDamper, damper);
	glUniform1f(m_iLocReflectivity, reflectivity);
}
///
/// Load models transformation matrix to the shader uniform locations
///
void StaticShader::loadModelTransform(glm::mat4 a_modelTransform)
{
	// mat4 needs to be converted to an opengl friendly format using 'value_ptr'
	glUniformMatrix4fv(m_iLocModelTansform, 1, GL_FALSE, glm::value_ptr(a_modelTransform));
}
///
/// Load Cameras view and projection matrices to the shader uniform locations
///
void StaticShader::loadCamera(Camera & a_camera)
{
	// mat4 needs to be converted to an opengl friendly format using 'value_ptr'
	glUniformMatrix4fv(m_iLocViewMatrix, 1, GL_FALSE, glm::value_ptr(a_camera.viewMatrix()));
	glUniformMatrix4fv(m_iLocProjMatrix, 1, GL_FALSE, glm::value_ptr(a_camera.projection()));
}
///
/// Load Lights position and colour to the shader uniform locations
///
void StaticShader::loadLight(Light & a_light)
{
	glUniform3f(m_iLocLightPos, a_light.position().x, a_light.position().y, a_light.position().z);
	glUniform3f(m_iLocLightColour, a_light.colour().x, a_light.colour().y, a_light.colour().z);
}
///
/// Load Textures to the shader uniform locations
///
void StaticShader::loadTextures(TexturedModel & a_texturedModel)
{
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, a_texturedModel.texture(0).ID());
	glUniform1i(m_iLocModelTexture, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, a_texturedModel.texture(1).ID());
	glUniform1i(m_iLocNormalTexture, 1);
}

