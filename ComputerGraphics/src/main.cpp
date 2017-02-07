#include <iostream>


// opengl includes
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>


int main()
{
	std::cout << "Hello World" << std::endl;

	if (!glfwInit()) {
		std::cout << "GLFW failed to initialise" << std::endl;
		return -1;
	}
	//// 4 x AntiAlising
	glfwWindowHint(GLFW_SAMPLES, 4);
	// set opengl version and profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// open a window
	GLFWwindow * window;
	window = glfwCreateWindow(640, 480, "OpengGL Template Project", NULL, NULL);

	if (!window) {
		std::cout << "window failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	// set current context to window
	glfwMakeContextCurrent(window);

	// draw triangle
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);



	do {
		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	std::cout << "Finished!! \n\n" << std::endl;

	return 0;
}