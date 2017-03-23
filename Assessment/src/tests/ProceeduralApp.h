#pragma once
#include "Application.h"
#include "OBJLoader.h"
#include "Light.h"
#include "StaticShader.h"
/// lights
#include "lights/LightShader.h"
#include "lights/PointLightShader.h"
#include "lights/SpotLightShader.h"
#include "lights/SoftSpotShader.h"

#include "MeshModel.h"
#include "Lamp.h"
#include "Square.h"
#include "Terrain.h"

#include "UIController.h"

class ProceeduralApp :
	public Application
{
public:
	ProceeduralApp();
	~ProceeduralApp();

	bool start();
	bool update(GLfloat a_deltaTime);
	bool fixedUpdate(GLfloat a_deltaTime);
	bool draw(GLfloat a_deltaTime);
	bool stop();

private:
	// private functions
	void setupLights();
	void setupModels();

	// display
	DisplayManager *	m_display = nullptr;
	Loader *			m_loader = nullptr;
	Renderer *			m_renderer = nullptr;
	Camera *			m_camera = nullptr;
	CameraController *	m_cameraController = nullptr;
	UIController *		m_uiController = nullptr;


	// shaders
	LightShader * m_lightSP = nullptr;
	PointLightShader * m_pLightSP = nullptr;
	SpotLightShader * m_sLightSP = nullptr;
	SoftSpotShader * m_ssLightSP = nullptr;
	ShaderProgram * m_terrainSP = nullptr;

	// textures
	Texture * m_whiteTexture = nullptr;
	Texture * m_lightTexture = nullptr;

	// models obj
	MeshModel * m_dragonModel = nullptr;
	MeshModel * m_lucyModel = nullptr;

	// models fbx
	MeshModel * m_duckModel = nullptr;


	// entities 
	Square * m_square = nullptr;
	Terrain * m_terrain = nullptr;

	// lights
	Lamp *	m_lamp = nullptr;
	Lamp *	m_pointLamp = nullptr;
	Lamp *	m_spotLamp = nullptr;
	Lamp *	m_softSpotLamp = nullptr;

	Light * m_light = nullptr;
	Light * m_pointLight = nullptr;
	Light * m_spotLight = nullptr;
	Light * m_softSpotLight = nullptr;

};

