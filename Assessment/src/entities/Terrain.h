#pragma once
// std includes
#include <algorithm>
#include <iterator>
// opengl includes
#include <glew.h>
#include <glm.hpp>
#include <gtc\noise.hpp>
// source includes
#include "ShaderProgram.h"
#include "DynamicModels.h"
#include "Texture.h"
#include "TexturedModel.h"
#include "Entity.h"

class Terrain
{
public:
	Terrain(glm::vec3 a_position, GLuint a_size);
	~Terrain();

	void draw(Camera & a_camera);
	glm::vec3 position() { return m_vPosition; };
	glm::vec3 position(glm::vec3 a_position) { m_vPosition = a_position; return m_vPosition; };
	glm::mat4 transform() { return m_m4Transform; };
	glm::mat4 transform(glm::mat4 a_transform) { m_m4Transform = a_transform; return m_m4Transform; };


	void toggleMaps() { 
		m_bToggleMap = true; 
		m_iMapIter < 3 ? m_iMapIter++ : m_iMapIter = 0;
	};

private:
	glm::vec3		m_vPosition;
	glm::mat4		m_m4Transform;
	glm::vec3		m_vTerrainColour;
	GLuint			m_uiSize;
	// default grid settings
	GLuint			m_uiGridSize;
	GLuint			m_uiVertNum;
	// map toggle
	int				m_iMapIter;
	bool			m_bToggleMap;
	
	RawModel *		m_terrainModel = nullptr;

	GLuint			m_perlinTexture;
	GLfloat			m_fShininess;

	ShaderProgram * m_shaderProgram = nullptr;
	
	Texture *		m_grassTexture = nullptr;
	Texture *		m_sandTexture = nullptr;
	Texture *		m_waterTexture = nullptr;
	Texture *		m_dirtTexture = nullptr;
	Texture *		m_snowTexture = nullptr;

	glm::vec3		m_vMapTex;

	TexturedModel *	m_terrainTM = nullptr;
	Entity *		m_terrain = nullptr;
	
	void setupTerrain();
	void perlinGeneration();
	void perlinGenNoisy();
	void perlinRings();

	void mapSelect();
};

