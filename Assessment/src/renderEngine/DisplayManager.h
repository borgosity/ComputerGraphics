#pragma once

#include <iostream>
// opengl includes
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
// source includes
#include "ConstValues.h"

class DisplayManager
{
public:
	DisplayManager();
	DisplayManager(char * a_title, GLint a_width, GLint a_height);
	~DisplayManager();

	void createDisplay();
	void updateDisplay();
	void closeDisplay();
	GLFWwindow * window() { return m_window; };

private:
	// Window dimensions
	GLFWwindow * m_window;
	char *		 m_cTitle;
	GLint		 m_iWidth;
	GLint		 m_iHeight;
};

