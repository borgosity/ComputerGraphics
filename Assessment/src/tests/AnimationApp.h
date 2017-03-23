#pragma once
#include "Application.h"
#include "OBJLoader.h"
#include "Light.h"

#include "StaticShader.h"
#include "AnimeShader.h"
/// lights
#include "lights/LightShader.h"
#include "lights/PointLightShader.h"
#include "lights/SpotLightShader.h"
#include "lights/SoftSpotShader.h"

// models
#include "AnimatedModel.h"
#include "MeshModel.h"
#include "Lamp.h"
#include "Square.h"
#include "Plane.h"

#include "UIController.h"

class AnimationApp :
	public Application
{
public:
	AnimationApp();
	~AnimationApp();

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

	AnimeShader * m_animeSP = nullptr;
	ShaderProgram * m_planeSP = nullptr;

	// textures
	Texture * m_whiteTexture = nullptr;
	Texture * m_lightTexture = nullptr;

	// models obj
	MeshModel * m_dragonModel = nullptr;
	MeshModel * m_handModel = nullptr;
	AnimatedModel * m_handAnimated = nullptr;

	// models fbx
	MeshModel * m_duckModel = nullptr;


	// entities 
	Square * m_square = nullptr;
	Plane * m_plane = nullptr;

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

