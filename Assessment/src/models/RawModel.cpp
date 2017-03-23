#include "RawModel.h"


RawModel::RawModel()
{
	m_iVaoID = 0;
	m_iVertexCount = 0;
	m_bIndices = false;
}

RawModel::RawModel(int vaoID, int vertCount )
{
	m_iVaoID = vaoID;
	m_iVertexCount = vertCount;
	m_bIndices = false;
}

RawModel::RawModel(int a_vaoID, int a_vertCount, bool a_indices)
{
	m_iVaoID = a_vaoID;
	m_iVertexCount = a_vertCount;
	m_bIndices = a_indices;
}


RawModel::~RawModel()
{
}

