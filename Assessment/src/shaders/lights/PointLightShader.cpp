#include "PointLightShader.h"



PointLightShader::PointLightShader()
{
}

PointLightShader::PointLightShader(ShaderPath a_shaderPath) : ShaderProgram(a_shaderPath)
{

}


PointLightShader::~PointLightShader()
{
}
/// ***************************************************************************************
///	Update
/// ***************************************************************************************
void PointLightShader::update(Camera & a_camera, Light & a_light)
{
	// update lighting
	uniformVec3("light.direction", a_light.direction());
	uniformVec3("viewPos", a_camera.position());
	// directional
	uniformVec3("light.ambient", a_light.ambient());
	uniformVec3("light.diffuse", a_light.diffuse());
	uniformVec3("light.specular", a_light.specular());
	// point
	uniformFloat("light.constant", a_light.linear());
	uniformFloat("light.linear", a_light.linear());
	uniformFloat("light.quadratic", a_light.quadratic());
	// update view and projection
	uniformMat4("view", a_camera.viewMatrix());
	uniformMat4("projection", a_camera.projection());
}



