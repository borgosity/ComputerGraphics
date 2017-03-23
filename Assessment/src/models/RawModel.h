#pragma once
// opnegl includes
#include <glew.h>
// source includes
#include "AABB.h"
#include "BoundingSphere.h"

class RawModel
{
public:
	RawModel();
	RawModel(int vaoID, int vertextCount);
	RawModel(int a_vaoID, int a_vertextCount, bool a_indices);
	~RawModel();

	GLuint vaoID() { return m_iVaoID; };
	GLuint vertexCount() { return m_iVertexCount; };
	bool hasIndecies() { return m_bIndices; };
	bool hasIndecies(bool a_value) { m_bIndices = a_value; return m_bIndices; };;

	AABB * aabb() { return m_aabb;};
	BoundingSphere * boundingSphere() { return m_bSphere; };
	AABB * aabb(AABB * a_aabb) { m_aabb = a_aabb; return m_aabb; };
	BoundingSphere * boundingSphere(BoundingSphere * a_bSphere) { m_bSphere = a_bSphere; return m_bSphere; };

private:
	GLuint		m_iVaoID;
	GLuint		m_iVertexCount;
	bool		m_bIndices; // does the model use indices
	AABB *		m_aabb = nullptr;
	BoundingSphere * m_bSphere = nullptr;
};

