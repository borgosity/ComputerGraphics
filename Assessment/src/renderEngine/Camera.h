#pragma once
// std library includes
#include <iostream>
// opengl includes
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
// source includes
#include "ConstValues.h"
#include "Maths.h"


class Camera
{
public:
	Camera();
	Camera(glm::vec3 a_cameraPostion, GLfloat a_speed, GLfloat a_fov, GLfloat a_nearView, GLfloat a_farView);
	~Camera();

	void update(GLfloat a_dt);

	// camera movement
	void moveForward(GLfloat a_dt);
	void moveBack(GLfloat a_dt);
	void moveLeft(GLfloat a_dt);
	void moveRight(GLfloat a_dt);
	// camera angles
	void pitchUpdate(GLfloat a_yOffset);
	void yawUpdate(GLfloat a_xOffset);
	// camera zoom
	void zoomUpdate(GLfloat a_yOffset);

	// update camera fowrard direction
	void updateFront();
	// update camera perspective 
	void updateProjection(GLfloat a_fov, GLfloat a_nearPlane, GLfloat a_farPlane);

	// getters
	glm::mat4	projection() { return m_mProjection; };
	glm::mat4	projection(glm::mat4 a_erspective) { return m_mProjection; };
	glm::mat4	viewMatrix() { return m_mViewMatrix; };
	glm::mat4	viewMatrix(glm::mat4 a_viewMatrix) { return m_mViewMatrix; };
	glm::vec3	position() { return m_vPosition; };
	glm::vec3	front() { return m_vFront; };
	glm::mat4	transform() { return Maths::createTransormationMatrix(m_vPosition, 
																	  glm::vec3(m_fPitch, m_fYaw, m_fRoll), 
																	  1.0f); };
	// eulers
	GLfloat		pitch() { return m_fPitch; };
	GLfloat		yaw() { return m_fYaw; };
	GLfloat		roll() { return m_fRoll; };

private:
	// shader uniform matrices
	glm::mat4	m_mProjection;
	glm::mat4	m_mViewMatrix;
	// camera movement
	glm::vec3	m_vFront;
	glm::vec3	m_vPosition;
	glm::vec3	m_vUp;
	GLfloat		m_fSpeed;
	// euler angles 
	GLfloat		m_fPitch;
	GLfloat		m_fYaw;
	GLfloat		m_fRoll;
	// camera perspective
	GLfloat		m_fFOV;
	GLfloat		m_fNearPlane;
	GLfloat		m_fFarPlane;
};

