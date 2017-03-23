#include "AABB.h"



AABB::AABB()
{
	reset();
}

AABB::AABB(GLfloat * a_vertices, int a_sizeof, int a_vertexSize)
{
	int size = (a_sizeof / sizeof(GLfloat));
	int increment = a_vertexSize - 3;
	std::vector<glm::vec3> points = {};
	// generate vector from vertices
	for (int i = 0; i < size; i++)
	{
		glm::vec3 vec(0);
		vec.x = a_vertices[i];
		vec.y = a_vertices[i++];
		vec.z = a_vertices[i++];
		points.push_back(vec);
		i += increment;
	}
	// set AABB
	set(points);
}


AABB::~AABB()
{
}
///
/// Reset minimum and maximum vec3's
///
void AABB::reset()
{
	m_vMin = glm::vec3(1e37f);
	m_vMax = glm::vec3(-1e37f);
}
///
/// Set minimum and maximum edges
///
void AABB::set(const std::vector<glm::vec3>& a_points)
{
	for (auto& p : a_points)
	{
		if (p.x < m_vMin.x) m_vMin.x = p.x;
		if (p.y < m_vMin.y) m_vMin.y = p.y;
		if (p.z < m_vMin.z) m_vMin.z = p.z;
		if (p.x > m_vMax.x) m_vMax.x = p.x;
		if (p.y > m_vMax.y) m_vMax.y = p.y;
		if (p.z > m_vMax.z) m_vMax.z = p.z;
	}	
}
