#pragma once
// std includes
#include <vector>
// opengl includes
#include <glew.h>
#include <glm.hpp>
// source includes
#include "SceneLights.h"


class SceneLights
{
public:
	SceneLights();
	SceneLights(std::vector<SceneLights*> a_dirSceneLights , 
				std::vector<SceneLights*> a_pointSceneLights, 
				std::vector<SceneLights*> a_spotSceneLights);

	~SceneLights();

	// getters
	std::vector<SceneLights*> directionalLights() { return m_dirSceneLights; };
	std::vector<SceneLights*> pointLights() { return m_pointSceneLights; };
	std::vector<SceneLights*> spotLights() { return m_spotSceneLights; };

private:
	// directional/general
	std::vector<SceneLights*> m_dirSceneLights;
	std::vector<SceneLights*> m_pointSceneLights;
	std::vector<SceneLights*> m_spotSceneLights;
};

