#pragma once
// std library includes
#include <vector>
// opengl includes
#include <glew.h>
#include <glm.hpp>

class AABB
{
public:
	AABB();
	AABB(GLfloat * a_vertices, int a_sizeof, int a_vertexSize);
	~AABB();

	void reset();
	void set(const std::vector<glm::vec3> & a_points);

	glm::vec3 min() { return m_vMin; };
	glm::vec3 max() { return m_vMax; };

private:
	glm::vec3 m_vMin;
	glm::vec3 m_vMax;

};

