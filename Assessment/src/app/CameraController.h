#pragma once
#include "Controller.h"
#include "ConstValues.h"


class CameraController :
	public Controller
{
public:
	CameraController();
	virtual ~CameraController();

	void update(Camera & camera, GLdouble a_dt);

	void keyPress(Camera & a_camera, GLfloat a_dt) override;
	void mouseMovement(Camera & a_camera) override;
	void mouseScroll(Camera & a_camera) override;
private:
	GLfloat m_fOldXoffset;
	GLfloat m_fOldYoffset;
	GLfloat m_fOldScrollOffset;

};

