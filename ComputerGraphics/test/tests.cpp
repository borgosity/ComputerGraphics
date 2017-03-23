#pragma once
#include "tests.h"
#include <iostream>

typedef aie::Gizmos AieGizmo;

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
	// ogl functions
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		std::cout << "--> ogl_LoadFunctions failed" << std::endl;
		return false;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL version: %i.%i\n", major, minor);

	// Gizmos code
	AieGizmo::create(10,10,10,10);
	glm::mat4 view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16/9.f, 0.1f, 1000.f);

	// draw loop
	while (!glfwWindowShouldClose(window))
	{
		// Check for key strokes
		glfwSetKeyCallback(window, key_callback);

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw gizmos
		AieGizmo::clear();
		AieGizmo::addTransform(glm::mat4(1));
		glm::vec4 white(1);
		glm::vec4 black(0, 0, 0, 1);
		for (int i = 0; i < 21; ++i) {
			AieGizmo::addLine(glm::vec3(-10 + i, 0, 10),
				glm::vec3(-10 + i, 0, -10),
				i == 10 ? white : black);
			AieGizmo::addLine(glm::vec3(10, 0, -10 + i),
				glm::vec3(-10, 0, -10 + i),
				i == 10 ? white : black);
		}
		AieGizmo::draw(projection * view);

		// Swap the screen buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	//cleanup
	cleanUp(window);

	// successful test, return true 
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

void cleanUp(GLFWwindow * window)
{
	// gizmo clenaup
	AieGizmo::destroy();
	// Terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
}

