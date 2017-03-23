#include "CameraController.h"



CameraController::CameraController()
{
}


CameraController::~CameraController()
{
}
/// update  Camera Controller
void CameraController::update(Camera & a_camera, GLdouble a_dt)
{
	// check for use input
	keyPress(a_camera, (GLfloat)a_dt);
	mouseMovement(a_camera);
	mouseScroll(a_camera);
	// update camera after input updates
	a_camera.update((GLfloat)a_dt);
}
/// User Keypress check
void CameraController::keyPress(Camera & a_camera, GLfloat a_dt)
{
	// check if keys state has changed
	if (m_bKeys[GLFW_KEY_W])
		a_camera.moveForward(a_dt);
	if (m_bKeys[GLFW_KEY_S])
		a_camera.moveBack(a_dt);
	if (m_bKeys[GLFW_KEY_A])
		a_camera.moveLeft(a_dt);
	if (m_bKeys[GLFW_KEY_D])
		a_camera.moveRight(a_dt);
}
/// User mouse movement check
void CameraController::mouseMovement(Camera & a_camera)
{
	// Update Camera yaw if offset has changed
	if (m_fOldXoffset != m_fXoffset) {
		a_camera.yawUpdate(m_fXoffset * m_fMouseSpeed);
		m_fOldXoffset = m_fXoffset;
	}

	// Update Camera Pitch if offset has changed
	if (m_fOldYoffset != m_fYoffset) {
		a_camera.pitchUpdate(m_fYoffset * m_fMouseSpeed);
		m_fOldYoffset = m_fYoffset;
	}
}
/// User mouse scroll check
void CameraController::mouseScroll(Camera & a_camera)
{
	// Update Camera Zoom if offset has changed
	if (m_fScrollYoffset != 0) {
		a_camera.zoomUpdate(m_fScrollYoffset * 0.025f);
		m_fScrollYoffset = 0;
	}
}
