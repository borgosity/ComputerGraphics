#include "SceneLights.h"



SceneLights::SceneLights()
{
	m_dirSceneLights = {};
	m_pointSceneLights = {};
	m_spotSceneLights = {};
}

SceneLights::SceneLights(std::vector<SceneLights*> a_dirSceneLights, std::vector<SceneLights*> a_pointSceneLights, std::vector<SceneLights*> a_spotSceneLights)
{
	m_dirSceneLights = a_dirSceneLights;
	m_pointSceneLights = a_pointSceneLights;
	m_spotSceneLights = a_spotSceneLights;
}

SceneLights::~SceneLights()
{
}
