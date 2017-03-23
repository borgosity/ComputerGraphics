#include "Camera.h"



Camera::Camera()
{
	m_vPosition = glm::vec3(0, 0.5f, -2.0f);
	m_fPitch = 0.0f;
	m_fYaw = 0.0f; //set some initial offset to avoid camera pointing to the right
	m_fRoll = 0.0f;
	m_fSpeed = 5.0f;
	m_vFront = glm::vec3(0.0f, 0.0f, -1.0f);
	m_vUp = glm::vec3(0.0f, 0.0f, 0.0f);
	// initialise perspective matrix
	m_fFOV = FOV;
	m_fNearPlane = NEAR_PLANE;
	m_fFarPlane = FAR_PLANE;
	m_mProjection = glm::perspective(m_fFOV, (GLfloat)SCREEN_W / (GLfloat)SCREEN_H, m_fNearPlane, m_fFarPlane);

}

Camera::Camera(glm::vec3 a_cameraPostion, GLfloat a_speed, GLfloat a_fov, GLfloat a_nearPlane, GLfloat a_farPlane)
{
	m_vPosition = a_cameraPostion;
	m_fPitch = 0.0f;
	m_fYaw = 0.0f; //set some initial offset to avoid camera pointing to the right
	m_fRoll = 0.0f;
	m_fSpeed = a_speed;
	m_vFront = glm::vec3(0.0f, 0.0f, -1.0f);
	m_vUp = glm::vec3(0.0f, 0.1f, 0.0f);
	// initialise perspective matrix
	m_fFOV = a_fov;
	m_fNearPlane = a_nearPlane;
	m_fFarPlane = a_farPlane;
	m_mProjection = glm::perspective(glm::radians(m_fFOV), (GLfloat)SCREEN_W / (GLfloat)SCREEN_H, m_fNearPlane, m_fFarPlane);
}

Camera::~Camera()
{
}
/// update camera
void Camera::update(GLfloat a_dt)
{
	// update camera view matrix
	m_mViewMatrix = glm::lookAt(m_vPosition, m_vPosition + m_vFront, m_vUp);
}
/// move camera into the scene
void Camera::moveForward(GLfloat a_dt)
{
	m_vPosition += (m_fSpeed * a_dt) * m_vFront;
}
/// move camera out of the scene
void Camera::moveBack(GLfloat a_dt)
{
	m_vPosition -= (m_fSpeed * a_dt) * m_vFront;
}
/// move camera to the left of the scene
void Camera::moveLeft(GLfloat a_dt)
{
	m_vPosition -= glm::normalize(glm::cross(m_vFront, m_vUp)) * (m_fSpeed * a_dt);
}
/// move camera to the right of the scene
void Camera::moveRight(GLfloat a_dt)
{
	m_vPosition += glm::normalize(glm::cross(m_vFront, m_vUp)) * (m_fSpeed * a_dt);
}
/// update the cameras pitch
void Camera::pitchUpdate(GLfloat a_yOffset)
{
	// increase pitch by offset
	m_fPitch += a_yOffset;
	// constrain up/down mouse movement
	if (m_fPitch > PITCH_MAX) m_fPitch = PITCH_MAX;
	if (m_fPitch < PITCH_MIN) m_fPitch = PITCH_MIN;
	// update cameras front
	updateFront();
}
/// update the cameras yaw
void Camera::yawUpdate(GLfloat a_xOffset)
{
	// increase yaw by offset, mod used to stop high values
	m_fYaw = glm::mod(m_fYaw + a_xOffset, 360.0f);
	// update cameras front
	updateFront();
}
/// update camera zoom
void Camera::zoomUpdate(GLfloat a_yOffset)
{
	// update field of view is within constraints
	if (m_fFOV >= FOV_MIN && m_fFOV <= FOV_MAX) {
		m_fFOV += a_yOffset;
	}
	// constrain field of view
	if (m_fFOV <= FOV_MIN)	m_fFOV = FOV_MIN;
	if (m_fFOV >= FOV_MAX) m_fFOV = FOV_MAX;
	// update camera projection
	updateProjection(m_fFOV, 0, 0);
}
/// 
/// Camera Front function calculates the direction the camera should face
///
void Camera::updateFront()
{
	glm::vec3 front;
	front.x = glm::sin(glm::radians(m_fYaw)) * glm::cos(glm::radians(m_fPitch));
	front.y = glm::sin(glm::radians(m_fPitch));
	front.z = -glm::cos(glm::radians(m_fYaw)) * glm::cos(glm::radians(m_fPitch));
	// normalise final vector
	m_vFront = glm::normalize(front);
}
///
/// Function to update the cameras perspecitve matrix
/// - zero values (0) passed will keep the original value
///
void Camera::updateProjection(GLfloat a_fov, GLfloat a_nearPlane, GLfloat a_farPlane)
{
	// check if 0 has been passed
	if (a_fov != 0) m_fFOV = a_fov;
	if (a_nearPlane != 0) m_fNearPlane = a_nearPlane;
	if (a_farPlane != 0) m_fFarPlane = a_farPlane;
	// update perspective with new value
	m_mProjection = glm::perspective(glm::radians(m_fFOV), (GLfloat)SCREEN_W / (GLfloat)SCREEN_H, m_fNearPlane, m_fFarPlane);
}


