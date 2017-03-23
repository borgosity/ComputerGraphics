#include "ParticleShader.h"



ParticleShader::ParticleShader()
{
}

ParticleShader::ParticleShader(ShaderPath a_shaderPath) : ShaderProgram(a_shaderPath)
{

}


ParticleShader::~ParticleShader()
{
}

void ParticleShader::update(Camera & a_camera, Light & a_light)
{
	// update lighting
	uniformMat4("view", a_camera.viewMatrix());
	// pass camera projection to shader
	uniformMat4("projection", a_camera.projection());
	// set lamp object colour
	uniformVec3("lightPos", glm::vec3(0.0f, 25.0f, 0.0f));
	uniformVec3("viewPos", a_camera.position());
	uniformBool("blinn", 0);
	// animation key frame time
	//std::cout << cosf(glfwGetTime() * 3) * 0.5f + 0.5f << std::endl;
	uniformFloat("keyTime", cosf((float)glfwGetTime() * 3) * 0.5f + 0.5f);
}



