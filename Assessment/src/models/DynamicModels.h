#pragma once
// opengl includes
#include <glew.h>
//#include <glfw3.h>
#include <glm.hpp>
// source includes
#include "Loader.h"
#include "AABB.h"
#include "BoundingSphere.h"


class DynamicModels
{
public:
	DynamicModels();
	~DynamicModels();

	static void grid(RawModel & a_rawmodel, GLuint a_size, GLuint a_vertCount);
	static void squareTBN(RawModel & a_rawmodel, GLfloat a_size);
	static RawModel * cube();
	static RawModel * square(GLfloat a_size);

private:
	static Loader * m_loader;
	//static Vertex * m_vertices;
	//static unsigned int * m_indices;
};

