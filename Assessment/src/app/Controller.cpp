#include "Controller.h"

// key state
GLboolean	Controller::m_bKeys[1024];
bool		Controller::m_bClose = false;
// mouse movement
bool		Controller::m_bInitialPos = false;
GLfloat		Controller::m_fOldXpos = HALFSC_W;
GLfloat		Controller::m_fOldYpos = HALFSC_H;
GLfloat		Controller::m_fXoffset = 0;
GLfloat		Controller::m_fYoffset = 0;
GLfloat		Controller::m_fMouseSpeed = MOUSE_SPEED;

// scroll offsets
GLfloat		Controller::m_fScrollXoffset = 0;
GLfloat		Controller::m_fScrollYoffset = 0;
// debug
bool		Controller::m_bWireFrame = false;
int			Controller::m_iMapSelector = 0;

Controller::Controller()
{
}

Controller::~Controller()
{
}

void Controller::keyCallback(GLFWwindow * a_window, int a_key, int a_scanCode, int a_action, int a_mode)
{
	// close the application
	if (a_key == GLFW_KEY_ESCAPE && a_action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(a_window, GL_TRUE);
		m_bClose = true;
	}
	// update key states
	if (a_action == GLFW_PRESS) {
		m_bKeys[a_key] = true;
	}
	else if (a_action == GLFW_RELEASE) {
		m_bKeys[a_key] = false;
	}
	// toggle wireframe
	if (a_key == GLFW_KEY_L && a_action == GLFW_PRESS)
	{
		// toggle wireFrame bool
		m_bWireFrame = !m_bWireFrame;
		// toggle wire frame based on bool state
		m_bWireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Controller::mouseCallback(GLFWwindow* a_window, double a_xPos, double a_yPos)
{
	// set initial pos position, 
	// stops screen jump on start up when cursor is not centered
	if (!m_bInitialPos)
	{
		m_fOldXpos = (GLfloat)a_xPos;
		m_fOldYpos = (GLfloat)a_yPos;
		m_bInitialPos = true;
	}
	// set offsets
	m_fXoffset = (GLfloat)a_xPos - m_fOldXpos; 
	m_fYoffset = (GLfloat)a_yPos - m_fOldYpos; // This is inverse look, pull back for looking up
	// set old position
	m_fOldXpos = (GLfloat)a_xPos;
	m_fOldYpos = (GLfloat)a_yPos;
}


void Controller::scrollCallback(GLFWwindow* a_window, double a_xOffset, double a_yOffset)
{
	m_fScrollXoffset = (GLfloat)a_xOffset;
	m_fScrollYoffset = (GLfloat)a_yOffset;
}
