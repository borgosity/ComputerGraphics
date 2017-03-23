#pragma once
#include "Controller.h"
#include "ConstValues.h"
#include "Terrain.h"


class UIController :
	public Controller
{
public:
	UIController();
	virtual ~UIController();

	void update(GLdouble a_dt);

	void keyPress(Camera & a_camera, GLfloat a_dt) override;
	void mouseMovement(Camera & a_camera) override;
	void mouseScroll(Camera & a_camera) override;
	void keyPress(GLfloat a_dt);

	bool toggleMap() { return m_bMapToggle; };
	bool toggleMap(bool a_toggle) { m_bMapToggle = a_toggle; return m_bMapToggle; };

	bool toggleCursor() { return m_bCursorToggle; };

private:
	bool m_bMapToggle = false;
	bool m_bCursorToggle = false;

};

