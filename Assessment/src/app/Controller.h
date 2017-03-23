#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
// source includes
#include "Camera.h"
#include "ConstValues.h"

class Controller
{
public:
	Controller();
	virtual ~Controller();

	static bool close() { return m_bClose; };
	// user input callbacks
	static void keyCallback(GLFWwindow* a_window, int a_key, int a_scanCode, int a_action, int a_mode);
	static void mouseCallback(GLFWwindow * a_window, double a_xPos, double a_yPos);
	static void scrollCallback(GLFWwindow * a_window, double a_xOffset, double a_yOffset);

protected:
	// key states
	static GLboolean m_bKeys[1024];
	static bool		 m_bClose;
	// mouse privates
	static bool		m_bInitialPos;
	static GLfloat	m_fOldXpos;
	static GLfloat	m_fOldYpos;
	static GLfloat	m_fXoffset;
	static GLfloat	m_fYoffset;
	static GLfloat  m_fMouseSpeed;
	// scroll offsets
	static GLfloat	m_fScrollXoffset;
	static GLfloat	m_fScrollYoffset;
	// debug variables
	static bool m_bWireFrame;
	static int m_iMapSelector;

	// protected functions
	virtual void keyPress(Camera & a_camera, GLfloat a_dt) = 0;
	virtual void mouseMovement(Camera & a_camera) = 0;
	virtual void mouseScroll(Camera & a_camera) = 0;
};

