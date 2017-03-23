#pragma once
// opengl includes
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
// depency includes
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
// source includes
#include "Controller.h"
// display
#include "DisplayManager.h"
#include "Loader.h"
#include "Renderer.h"
#include "Camera.h"
// utility
#include "Maths.h"
#include "CameraController.h"
// shaders
#include "ShaderProgram.h"
#include "Shaders.h"
// textures
#include "ModelTexture.h"
#include "TexturedModel.h"
#include "Texture.h"
// models
#include "Entity.h"
#include "DynamicModels.h"

class Application
{
public:
	Application();
	~Application();

	bool run();
	virtual bool init();
	virtual bool start();
	virtual bool update(GLdouble a_deltaTime);
	virtual bool fixedUpdate(GLdouble a_deltaTime);
	virtual bool draw(GLdouble a_deltaTime);
	virtual bool stop();

protected:
	void deltaTime();

	template<typename T>
	void deallocate(T a_ptr);

private:
	// time
	GLdouble m_fDeltaTime;
	GLdouble m_fCurrentTime;
	GLdouble m_fPrevTime;

};

template<typename T>
inline void Application::deallocate(T a_ptr)
{
		delete a_ptr;
		a_ptr = nullptr;
}
