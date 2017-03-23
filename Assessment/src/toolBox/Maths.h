#pragma once

#include <glm.hpp>
#include <gtx\transform.hpp>
#include <gtc\quaternion.hpp>
#include <gtx\quaternion.hpp>
// source includes
//#include "Camera.h"

class Maths
{
public:
	Maths();
	~Maths();

	static glm::mat4 createTransormationMatrix(glm::vec3 a_translation, glm::vec3 a_rotation, float a_scale);
	//static glm::mat4 createViewMatrix(Camera & a_camera);
	static glm::mat4 createWorldTransformMatrix(glm::vec3 a_translation, glm::vec3 a_rotation, float a_scale);
	static glm::mat4 quaternionTransformation(glm::vec3 a_translation, glm::quat a_rotation, float a_scale);
	static float minElement(float * a_array, int a_size);
	static float maxElement(float * a_array, int a_size);
	static void frustrumPlanes(const glm::mat4& a_projection, 
							   const glm::mat4& a_view,
							   glm::vec4 * a_planes);


};

