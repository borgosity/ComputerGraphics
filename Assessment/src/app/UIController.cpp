#include "UIController.h"



UIController::UIController()
{
}


UIController::~UIController()
{
}
/// update  Camera Controller
void UIController::update(GLdouble a_dt)
{
	// check for use input
	keyPress((GLfloat)a_dt);

}
/// User Keypress check
void UIController::keyPress(Camera & a_camera, GLfloat a_dt)
{

}

void UIController::mouseMovement(Camera & a_camera)
{
}

void UIController::mouseScroll(Camera & a_camera)
{
}

void UIController::keyPress(GLfloat a_dt)
{
	// check if keys state has changed
	// - toggle maps
	if (m_bKeys[GLFW_KEY_M] == (GLboolean)true && !m_bMapToggle) {
		m_bMapToggle = true;
	}
	else if (m_bKeys[GLFW_KEY_M] == (GLboolean)false && m_bMapToggle) {
		m_bMapToggle = false;
	}

	// - toggle cursor to enable UI interaction if left ALT pressed
	if (m_bKeys[GLFW_KEY_LEFT_ALT] == (GLboolean)true && !m_bCursorToggle) {
		m_bCursorToggle = true;
	}
	else if (m_bKeys[GLFW_KEY_LEFT_ALT] == (GLboolean)false && m_bCursorToggle){
		m_bCursorToggle = false;
	}
}

