#pragma once
// std includes
#include <iostream>
#include <vector>
#include <list>
// gl includes
#include <glew.h>
#include <glm.hpp>
#include <SOIL.h>   // texturing
#include <assimp\Importer.hpp>
// local includes
#include "RawModel.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Vertex3
{
	GLfloat x, y, z;	   // position
	GLfloat nx, ny, nz;	   // normal
	GLfloat u, v;		   // texture co-ords
};						   

struct Vertex4
{
	GLfloat x, y, z;	// position
	GLfloat nx, ny, nz;	// normal
	GLfloat tx, ty, tz;	// tangent
	GLfloat u, v;		// texture co-ords
};

struct MeshTexture {
	GLuint id;
	std::string type;
	aiString path;
};

struct MeshCoords {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<MeshTexture> textures;
	GLfloat shininess;
};

class Loader
{
public:
	Loader();
	~Loader();

	RawModel * loadToVAO(GLfloat positions[], int size);
	RawModel * loadToVAO(GLfloat a_positions[], int a_posSize, int a_vertexSize);
	RawModel * loadToVAO(GLfloat a_positions[], int a_posSize, int a_vertexSize, GLuint a_indicies[], int a_indSize);
	
	template<typename T>
	RawModel & loadToVAO(T * a_positions, int a_posSize, int a_vertexSize, GLuint * a_indicies, int a_indSize);

	RawModel * loadToVAO(GLfloat a_positions[], int a_posSize, GLuint a_indicies[], int a_indSize);
	RawModel * loadToVAO(GLfloat a_positions[], int a_pSize, GLfloat a_colours[], int a_cSize, GLfloat a_textures[], int a_tSize, GLuint a_indicies[], int a_iSize);
	RawModel * loadTextureVAO(GLfloat a_positions[], int a_size, GLuint a_indicies[], int a_indSize);
	GLuint loadTexture(std::string a_fileName);
	void cleanUp();

private:
	
	std::list<GLuint> m_vVAOs;
	std::list<GLuint> m_vVBOs;
	std::list<GLuint> m_vTextures; // list of textures
	
	// private functions
	GLuint createVAO();
	void createVBO(GLfloat data[], int size);
	template<typename T>
	void createVBO(T * data, int size);
	void bindIndicesBuffer(GLuint indices[], int size);
	
	// store data functions
	void storePositionDataInAttributeList(int attributeNumber);
	void storeColourDataInAttributeList(int attributeNumber);
	void storeTextureDataInAttributeList(int attributeNumber);

	// refactor
	// single verticies functions
	void storePositionData(int attributeNumber, int vertSize, int startPos);
	void storeColourData(int attributeNumber, int vertSize, int startPos);
	void storeTextureData(int attributeNumber, int vertSize, int startPos);
	void storeTangentData(int attributeNumber, int vertSize, int startPos);

	// split verticies functions
	void storePositionData(int attributeNumber, GLfloat data[], int size);
	void storeColourData(int attributeNumber, GLfloat data[], int size);
	void storeTextureData(int attributeNumber, GLfloat data[], int size);

	void unbind();
};

///
/// Function to Load data to VAO - nullptr and 0 if there are no Indices
/// positions = vertex data ( it can contain	- position and colour and texture and tangent
///												- position and colour and texture 
///												- position and colour
///												- position and texture
///												- position ) 
/// posSize = size of vertex data array
/// vertexSize = size of the vertex buffer blocks	- 11 (p & c & t and t)
///													- 8 (p & c & t)
///													- 6 (p & C) 
///													- 5 (p & t) 
///													- 3 (p)
///
template<typename T>
inline RawModel & Loader::loadToVAO(T * a_positions, int a_posSize, int a_vertexSize, GLuint * a_indicies, int a_indSize)
{
	GLuint vaoID = createVAO();					// create and bind VAO
	createVBO(a_positions, a_posSize);			// create and bind VBOs
	
	// indices checks
	int vertCount = a_posSize / a_vertexSize;
	bool hasIndices = false;						// doesn't has indicies unless nullptr check fails
	// are the indices values
	if (a_indicies != nullptr) {
		bindIndicesBuffer(a_indicies, a_indSize);// create and bind indicies
		hasIndices = true;
		vertCount = a_indSize / sizeof(GLuint);
	}
	int startPos = 0;
	// assume there is always position data
	storePositionData(0, a_vertexSize, startPos);
	startPos += 3;									// add 3 to the startpos for the next attribute
	// if there is more than 1 set of 3 data assume second set is colour
	if (a_vertexSize / 3 >= 2) {
		storeColourData(1, a_vertexSize, startPos);
		startPos += 3;								// add 3 to the startpos for the next attribute
	}
	// if the data has a remainder of 3 then assume there is tangent data
	if (a_vertexSize / 3 >= 3) {
		storeTangentData(3, a_vertexSize, startPos);
		startPos += 3;								// add 3 to the startpos for the next attribute
	}
	// if the data has a remainder of 2 then assume there is texture data
	if (a_vertexSize % 3 == 2) {
		storeTextureData(2, a_vertexSize, startPos);
	}
	// unbind vbo and vao
	unbind();
	// return a RawModel object
	return RawModel(vaoID, vertCount, hasIndices);
}

template<typename T>
inline void Loader::createVBO(T * data, int size)
{
	GLuint vboID = 0;
	// bind and set vertex buffer(s) and attribute pointer(s).
	glGenBuffers(1, &vboID);
	m_vVBOs.push_back(vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
