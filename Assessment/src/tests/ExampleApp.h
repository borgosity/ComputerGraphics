#pragma once
#include "Application.h"
class ExampleApp :
	public Application
{
public:
	ExampleApp();
	~ExampleApp();

	bool start();
	bool update(GLfloat a_deltaTime);
	bool fixedUpdate(GLfloat a_deltaTime);
	bool draw(GLfloat a_deltaTime);
	bool stop();

private:
	// display
	DisplayManager *	m_display;
	Loader *			m_loader;
	Renderer *			m_renderer;
	Camera *			m_camera;
	CameraController *	m_cameraController;
	// textures
	ShaderProgram * m_shaderProgram;
	Texture *		m_sunTexture;
	Texture *		m_planetTexture;
	Texture *		m_moonTexture;
	// models
	RawModel *		m_cubeModel;
	RawModel *		m_squareModel;
	TexturedModel * m_sunModel;
	TexturedModel * m_planetModel;
	TexturedModel * m_moonModel;
	// planetary entities 
	Entity * m_sun;
	Entity * m_planet;
	Entity * m_moon;
};

