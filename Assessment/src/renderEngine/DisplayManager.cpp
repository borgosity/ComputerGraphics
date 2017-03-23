#include "DisplayManager.h"
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"


DisplayManager::DisplayManager()
{
	m_cTitle = "Used Default Constructor ;)";
	m_iHeight = SCREEN_H;
	m_iWidth = SCREEN_W;
}

DisplayManager::DisplayManager(char * a_title, GLint a_width, GLint a_height)
{
	m_cTitle = a_title;
	m_iHeight = a_height;
	m_iWidth = a_width;
}


DisplayManager::~DisplayManager()
{

}

void DisplayManager::createDisplay()
{
	// setup window options
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// create window
	m_window = glfwCreateWindow(m_iWidth, m_iHeight, m_cTitle, nullptr, nullptr);
	if (m_window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(m_window);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// initialise GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
	}

	// ==== GUI initialise =================
	ImGui_ImplGlfwGL3_Init(m_window, true);

	// define rendering viewport
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	glViewport(0, 0, width, height);

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);
}

void DisplayManager::updateDisplay()
{
	// swap buffers
	glfwSwapBuffers(m_window);
}

void DisplayManager::closeDisplay()
{

}
