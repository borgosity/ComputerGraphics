#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

// source includes
#include "DisplayManager.h"

// tests
void helloWorld();
void helloTriangle();
void renderEngineTest();

// miscellaneous functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void screenColour(GLfloat *red, GLfloat *green, GLfloat *blue, GLboolean *allColour);