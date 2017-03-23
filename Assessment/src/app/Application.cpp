#include "Application.h"



Application::Application()
{
	// time
	m_fDeltaTime = 0.0f;
	m_fCurrentTime = 0.0f;
	m_fPrevTime = 0.0f;
	// initialise 
	init();
}


Application::~Application()
{
}

bool Application::init()
{
	return true;
}

bool Application::start()
{
	return true;
}

bool Application::run()
{
	// initialise everything
	start();

	// application loop
	while (!Controller::close())
	{
		// update delta time
		deltaTime();
		// Check if any events have been activiated (key pressed, mouse moved etc.) 
		//  and call corresponding response functions
		glfwPollEvents();

		// application updates
		update(m_fDeltaTime);
		fixedUpdate(m_fDeltaTime);

		// draw application
		draw(m_fDeltaTime);
	}
	// stop application
	stop();

	return true;
}

bool Application::update(GLdouble a_deltaTime)
{
	return true;
}

bool Application::fixedUpdate(GLdouble a_deltaTime)
{
	return true;
}

bool Application::draw(GLdouble a_deltaTime)
{	
	return true;
}

bool Application::stop()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return true;
}

void Application::deltaTime()
{
	m_fCurrentTime = glfwGetTime();
	m_fDeltaTime = m_fCurrentTime - m_fPrevTime;
	m_fPrevTime = m_fCurrentTime;
}

