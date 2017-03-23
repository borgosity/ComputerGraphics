#pragma once
#include "Application.h"
#include "OBJLoader.h"
#include "Light.h"
#include "StaticShader.h"

class TerrainApp :
	public Application
{
public:
	TerrainApp();
	~TerrainApp();

	bool start();
	bool update(GLfloat a_deltaTime);
	bool fixedUpdate(GLfloat a_deltaTime);
	bool draw(GLfloat a_deltaTime);
	bool stop();

private:
	// display
	DisplayManager *	m_display = nullptr;
	Loader *			m_loader = nullptr;
	Renderer *			m_renderer = nullptr;
	Camera *			m_camera = nullptr;
	CameraController *	m_cameraController = nullptr;

	// shaders
	ShaderProgram * m_colourSP = nullptr;
	ShaderProgram * m_cameraSP = nullptr;
	ShaderProgram * m_phongSP = nullptr;
	ShaderProgram * m_modelSP = nullptr;
	ShaderProgram * m_rippleSP = nullptr;

	StaticShader * m_staticShader = nullptr;

	// textures
	Texture *		m_soulSpearTexture = nullptr;
	Texture *		m_soulSpearNormalMap = nullptr;
	Texture *		m_rocksTexture = nullptr;
	Texture *		m_rocksNormalMap = nullptr;
	Texture *		m_duckTexture = nullptr;
	Texture *		m_waterTexture = nullptr;

	// models
	RawModel *		m_gridModel = nullptr;
	RawModel *		m_cubeModel = nullptr;
	RawModel *		m_soulSpearRM = nullptr;
	RawModel *		m_duckRM = nullptr;
	RawModel *		m_rocksModel = nullptr;

	// textured models
	TexturedModel * m_waterTM = nullptr;
	TexturedModel * m_flagTM = nullptr;
	TexturedModel * m_soulSpearTM = nullptr;
	TexturedModel * m_duckTM = nullptr;
	TexturedModel * m_rocksTM = nullptr;

	// entities 
	Entity * m_soulSpear = nullptr;
	Entity * m_flag = nullptr;
	Entity * m_rubberDuck = nullptr;
	Entity * m_water = nullptr;
	Entity * m_rocksE = nullptr;

	// lights
	Light * m_duckLight = nullptr;


};

