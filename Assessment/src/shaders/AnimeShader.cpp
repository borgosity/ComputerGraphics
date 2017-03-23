#include "AnimeShader.h"



AnimeShader::AnimeShader()
{
}

AnimeShader::AnimeShader(ShaderPath a_shaderPath) : ShaderProgram(a_shaderPath)
{

}


AnimeShader::~AnimeShader()
{
}

void AnimeShader::update(Camera & a_camera, Light & a_light)
{
	// update lighting
	uniformMat4("view", a_camera.viewMatrix());
	// pass camera projection to shader
	uniformMat4("projection", a_camera.projection());
	// set lamp object colour
	uniformVec3("lightPos", m_lightPosition);
	uniformVec4("specColour", m_specularColour);
	uniformVec3("viewPos", a_camera.position());
	uniformBool("blinn", 0);
	// animation key frame time
	//std::cout << cosf(glfwGetTime() * 3) * 0.5f + 0.5f << std::endl;
	uniformFloat("keyTime", cosf((float)glfwGetTime() * 3) * 0.5f + 0.5f);
}



