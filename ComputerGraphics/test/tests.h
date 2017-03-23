#pragma once
#include <Gizmos.h>
#include <gl_core_4_4.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// source includes


// tests
bool helloWorld();

// miscellaneous functions
void cleanUp(GLFWwindow * window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

