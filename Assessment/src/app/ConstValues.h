#pragma once
#include <glew.h>
#include <math.h>

#define M_PI       3.14159265358979323846   // pi

// window properties

const GLint SCREEN_W = 1280;
const GLint SCREEN_H = 720;
const GLint HALFSC_W = SCREEN_W >> 1;
const GLint HALFSC_H = SCREEN_H >> 1;

// display properties
const GLint		MAX_FPS		= 120;
const GLfloat	FOV			= 55.0f;
const GLfloat	FOV_MAX		= FOV +1.5f;
const GLfloat	FOV_MIN		= FOV -1.5f;
const GLfloat	NEAR_PLANE	= 0.1f;
const GLfloat	FAR_PLANE	= 100.0f;

// controller properties
const GLfloat MOUSE_SPEED = 0.25f;
const GLfloat CAM_SPEED = 5.0f;
const GLfloat PITCH_MAX = 89.0f;
const GLfloat PITCH_MIN = -89.0f;