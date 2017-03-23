#include "LightShader.h"



LightShader::LightShader()
{
}

LightShader::LightShader(ShaderPath a_shaderPath) : ShaderProgram(a_shaderPath)
{

}


LightShader::~LightShader()
{
}

void LightShader::update(Camera & a_camera, Light & a_light)
{
	// update lighting
	uniformVec3("light.direction", a_light.direction());
	uniformVec3("viewPos", a_camera.position());
	uniformVec3("light.ambient", a_light.ambient());
	uniformVec3("light.diffuse", a_light.diffuse());
	uniformVec3("light.specular", a_light.specular());
	// update view and projection
	uniformMat4("view", a_camera.viewMatrix());
	uniformMat4("projection", a_camera.projection());
}



