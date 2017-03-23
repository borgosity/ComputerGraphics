#pragma once
#include "Application.h"
#include "OBJLoader.h"
#include "Light.h"
// shaders
#include "StaticShader.h"
#include "AnimeShader.h"
#include "particles\ParticleShader.h"
// lights
#include "lights/LightShader.h"
#include "lights/PointLightShader.h"
#include "lights/SpotLightShader.h"
#include "lights/SoftSpotShader.h"
// particles
#include "ParticleEmitter.h"
// models
#include "AnimatedModel.h"
#include "MeshModel.h"
#include "Lamp.h"
#include "Square.h"
#include "Plane.h"
#include "Mirror.h"
// controllers
#include "UIController.h"
// conlisions
#include "BoundingSphere.h"
#include "AABB.h"

class AssessmentApp :
	public Application
{
public:
	AssessmentApp();
	~AssessmentApp();

	bool start();
	bool update(GLdouble a_deltaTime);
	bool fixedUpdate(GLdouble a_deltaTime);
	bool draw(GLdouble a_deltaTime);
	bool stop();

private:
	// private functions
	void setupLights();
	void setupModels();
	void setupGUI();
	void debugGUI();
	bool culling();
	bool AABBculling(AABB & a_box);


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
	ParticleShader * m_particleSP = nullptr;

	ShaderProgram * m_planeSP = nullptr;

	// textures
	Texture * m_whiteTexture = nullptr;
	Texture * m_lightTexture = nullptr;

	// models obj
	MeshModel * m_dragonModel = nullptr;
	MeshModel * m_handModel = nullptr;
	AnimatedModel * m_handAnimated = nullptr;
	glm::vec3 m_handPosition;

	// models fbx
	MeshModel * m_duckModel = nullptr;

	// entities 
	Square * m_square = nullptr;
	Plane * m_plane = nullptr;
	Mirror * m_mirror = nullptr;

	// particles
	ParticleEmitter * m_emitter = nullptr;
	glm::vec4		m_particleColour;
	glm::vec4		m_emitterPosition;

	// lights
	Lamp *	m_lamp = nullptr;
	Lamp *	m_pointLamp = nullptr;
	Lamp *	m_spotLamp = nullptr;
	Lamp *	m_softSpotLamp = nullptr;

	Light * m_light = nullptr;
	Light * m_pointLight = nullptr;
	Light * m_spotLight = nullptr;
	Light * m_softSpotLight = nullptr;

	// GUI
	glm::vec4 m_clearColour;

	// collisions
	glm::vec4		 m_vPlanes[6];
	BoundingSphere * m_boundingSphere = nullptr;

};

