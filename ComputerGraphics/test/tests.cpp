#pragma once
#include "tests.h"
#include <iostream>


bool helloWorld()
{
	std::cout << "Starting OpenGL Test - 01" << std::endl;

	// create display
	if (glfwInit() == false) {
		std::cout << "--> glfwInit returned false" << std::endl;
		return false;
	}

	GLFWwindow * window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (window == nullptr) {
		glfwTerminate();
		std::cout << "--> window was nullptr" << std::endl;
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		std::cout << "--> ogl_LoadFunctions failed" << std::endl;
		return false;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL version: %i.%i\n", major, minor);

	// draw loop
	while (!glfwWindowShouldClose(window))
	{
		// Check for key strokes
		glfwSetKeyCallback(window, key_callback);

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap the screen buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	// Terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	return true;
	std::cout << "Finished!!  - 01 \n\n" << std::endl;
}

/// key press callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		std::cout << "Escape Key Pressed" << std::endl;
	}
}

