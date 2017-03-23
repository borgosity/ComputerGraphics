#pragma once
#include <sstream>
#include <vector>

// gl includes
#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// source includes
#include "Loader.h"
#include "ShaderProgram.h"

class Mesh
{
public:
	Mesh(std::vector<Vertex> a_vertices, std::vector<GLuint> a_indices, std::vector<MeshTexture> a_textures, float a_shininess);
	Mesh(MeshCoords * a_startMesh, MeshCoords * a_endMesh);
	~Mesh();

	void draw(ShaderProgram & a_shaderProgram);
	void animate(ShaderProgram & a_shaderProgram);

private:
	// mesh data
	std::vector<Vertex> m_vVertices;
	std::vector<GLuint> m_vIndices;
	std::vector<MeshTexture> m_vTextures;

	// animation mesh data
	MeshCoords * m_startMesh = nullptr;
	MeshCoords * m_endMesh = nullptr;
	
	// Buffers
	GLuint VAO, VBO1, VBO2, EBO1, EBO2;

	// attributes
	GLfloat m_fShininess;

	void setupMesh();
	void mergeMeshData();
};

