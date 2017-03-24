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
	void destroy();

private:
	// private functions
	void objectDraw(GLdouble a_deltaTime);
	void setupLights();
	void setupModels();
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

	// models obj
	MeshModel * m_dragonModel = nullptr;
	glm::vec3 m_dragonPosition;
	MeshModel * m_lucyModel = nullptr;
	glm::vec3 m_lucyPosition;
	MeshModel * m_handModel = nullptr;
	AnimatedModel * m_handAnimated = nullptr;
	glm::vec3 m_handPosition;

	// models fbx
	MeshModel * m_duckModel = nullptr;
	glm::vec3 m_duckPosition;

	// entities 
	Plane * m_plane = nullptr;
	Mirror * m_mirror = nullptr;

	// particles
	ParticleEmitter * m_emitter = nullptr;
	glm::vec4		m_particleColour;
	glm::vec3		m_emitterPosition;

	// lamp objects
	Lamp *	m_dragonlamp = nullptr;
	Lamp *	m_lucyLamp = nullptr;
	Lamp *	m_duckLamp = nullptr;
	Lamp *	m_planeLamp = nullptr;
	// light objects
	Light * m_light = nullptr;
	Light * m_pointLight = nullptr;
	Light * m_spotLight = nullptr;
	Light * m_softSpotLight = nullptr;

	// GUI
	glm::vec4 m_clearColour;
	static bool m_blur;
	static bool m_invert;
	static bool m_greyScale;
	static bool m_sharpen;
	static bool m_edgeDetect;

	// collisions/culling
	glm::vec4		 m_vPlanes[6];
	BoundingSphere * m_boundingSphere = nullptr;

};

