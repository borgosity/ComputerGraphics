#include "BoundingSphere.h"



BoundingSphere::BoundingSphere()
{
	m_vCentre = glm::vec3(0.0f);
	m_fRadius = 0.0f;
}

BoundingSphere::BoundingSphere(glm::vec3 a_centre, float a_radius)
{
	m_vCentre = a_centre;
	m_fRadius = a_radius;
}


BoundingSphere::~BoundingSphere()
{
}

void BoundingSphere::set(const std::vector<glm::vec3>& a_points)
{
	glm::vec3 min(1e37f);
	glm::vec3 max(-1e37f);

	for (auto& p : a_points)
	{
		if (p.x < min.x) min.x = p.x;
		if (p.y < min.y) min.y = p.y;
		if (p.z < min.z) min.z = p.z;
		if (p.x > max.x) max.x = p.x;
		if (p.y > max.y) max.y = p.y;
		if (p.z > max.z) max.z = p.z;
	}

	m_vCentre = (min + max) * 0.5f;
	m_fRadius = glm::distance(min, m_vCentre);
}
