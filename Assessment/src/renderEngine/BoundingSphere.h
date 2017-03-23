#pragma once
// std library includes
#include <vector>
// opengl includes
#include <glm.hpp>

class BoundingSphere
{
public:
	BoundingSphere();
	BoundingSphere(glm::vec3 a_centre, float a_radius);

	~BoundingSphere();

	void set(const std::vector<glm::vec3> & points);

	glm::vec3 centre() { return m_vCentre; };
	glm::vec3 centre(glm::vec3 a_value) { m_vCentre = a_value; return m_vCentre; };
	float radius() { return m_fRadius; };
	float radius(float a_value) { m_fRadius = a_value; return m_fRadius; };

private:
	glm::vec3	m_vCentre;
	float		m_fRadius;
};

