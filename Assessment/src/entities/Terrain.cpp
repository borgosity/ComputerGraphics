#include "Terrain.h"



Terrain::Terrain(glm::vec3 a_position, GLuint a_size)
{
	m_vPosition = a_position;
	m_shaderProgram = new ShaderProgram(Shader::terrainShader);
	m_vTerrainColour = glm::vec3(1.0f, 1.0f, 1.0f);
	// terrain grid
	m_uiGridSize = a_size;
	m_uiVertNum = a_size;
	m_m4Transform = Maths::createTransormationMatrix(glm::vec3(-32.0f, 0.0f, -32.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	m_terrainModel = new RawModel();

	// textures
	m_grassTexture = new Texture("res/textures/grass.png");
	m_sandTexture = new Texture("res/textures/sand.png");
	m_waterTexture = new Texture("res/textures/water.png");
	m_dirtTexture = new Texture("res/textures/dirt.png");
	m_snowTexture = new Texture("res/textures/snow.png");
	
	m_fShininess = 16.0f;
	
	setupTerrain();

	m_iMapIter = 0;
	m_bToggleMap = false;
	mapSelect();
}


Terrain::~Terrain()
{
	delete m_terrainModel;
	// set ptrs to null
	m_terrainModel = nullptr;
}

void Terrain::draw(Camera & a_camera)
{
	// toggle map if requested
	if (m_bToggleMap)
		mapSelect();

	float time = (float)glfwGetTime();

	//m_m4Transform = Maths::createWorldTransformMatrix(m_vPosition, glm::vec3(0.0f, time * 15.0f, 0.0f), 1.0f);

	m_shaderProgram->start();
	// pass camera position to shader 
	m_shaderProgram->uniformMat4("view", a_camera.viewMatrix());
	// pass camera projection to shader
	m_shaderProgram->uniformMat4("projection", a_camera.projection());
	// pass transform to shader
	m_shaderProgram->uniformMat4("model", m_m4Transform);
	// set lamp object colour
	m_shaderProgram->uniformVec3("lightPos", glm::vec3(0.0f, 25.0f, 0.0f));
	m_shaderProgram->uniformVec3("viewPos", a_camera.position());
	m_shaderProgram->uniformBool("blinn", 0);
	m_shaderProgram->uniformFloat("time", time);
	// texture mapping heights
	m_shaderProgram->uniformVec3("mapTex", m_vMapTex);


	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);
	glUniform1i(glGetUniformLocation(m_shaderProgram->ID(), "texture_perlin"), 0);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_grassTexture->ID());
	glUniform1i(glGetUniformLocation(m_shaderProgram->ID(), "texture_grass"), 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_sandTexture->ID());
	glUniform1i(glGetUniformLocation(m_shaderProgram->ID(), "texture_sand"), 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_waterTexture->ID());
	glUniform1i(glGetUniformLocation(m_shaderProgram->ID(), "texture_water"), 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_dirtTexture->ID());
	glUniform1i(glGetUniformLocation(m_shaderProgram->ID(), "texture_dirt"), 4);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_snowTexture->ID());
	glUniform1i(glGetUniformLocation(m_shaderProgram->ID(), "texture_snow"), 5);

	// bind vertex array
	glBindVertexArray(m_terrainModel->vaoID());
	// draw arrays
	glDrawElements(GL_TRIANGLES, m_terrainModel->vertexCount(), GL_UNSIGNED_INT, 0);
	// unbind vertex array
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// stop shader
	m_shaderProgram->stop();
}

void Terrain::setupTerrain()
{

	DynamicModels::grid(*m_terrainModel, m_uiGridSize, m_uiVertNum);

}

void Terrain::perlinGeneration()
{
	// texture mapping heights
	m_vMapTex = glm::vec3(0.25f, 0.55f, 0.05f);
	// create perlin data
	int size = m_uiVertNum * m_uiVertNum;
	float* perlinData = new float[size];
	float scale = (1.0f / m_uiVertNum) * 8;
	int octaves = 6;

	for  (unsigned int x = 0; x < m_uiVertNum; x++)
	{
		for (unsigned int y = 0; y < m_uiVertNum; y++)
		{
			//std::cout << glm::perlin(glm::vec2(x, y) * scale) * 0.5f + 0.5f << std::endl;
			perlinData[y* m_uiVertNum + x] = glm::perlin(glm::vec2(x, y) * scale) * 0.5f + 0.5f;
		}
	}


	// generate texture
	glGenTextures(1, &m_perlinTexture);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_uiVertNum, m_uiVertNum, 0, GL_RED, GL_FLOAT, perlinData);

	delete[] perlinData;
}

void Terrain::perlinGenNoisy()
{
	// texture mapping heights
	m_vMapTex = glm::vec3(0.85f, 0.95f, 0.05f);
	// create perlin data
	int size = m_uiVertNum * m_uiVertNum;
	float* perlinData = new float[size];
	float scale = (1.0f / m_uiVertNum) * 3;
	int octaves = 2;

	for (unsigned int x = 0; x < m_uiVertNum; x++)
	{
		for (unsigned int y = 0; y < m_uiVertNum; y++)
		{
			float amplitude = 1.0f;
			float persistance = 0.3f;
			perlinData[y * m_uiVertNum + x] = 0;

			for (int o = 0; o < octaves; o++)
			{
				float freq = powf(2, (float)o);
				float perlinSample = glm::perlin(glm::vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;
				perlinData[y * m_uiVertNum + x] += perlinSample + amplitude;
				//std::cout << (perlinData[y * m_uiVertNum + x]) / 10 << std::endl;
				amplitude *= persistance;
			}
		}
	}
	//---------- normalise values to between 0 and 1
	// get min and max values
	//float maxValue = Maths::maxElement(perlinData, size);
	//float minValue = Maths::minElement(perlinData, size);
	//// get the difference
	//float diff = maxValue - minValue;
	//// convert values
	//for (int i = 0; i < size; i++)
	//{
	//	float result = perlinData[i] / diff;
	//	perlinData[i] = result;
	//	std::cout << perlinData[i] << std::endl;
	//}


	// generate texture
	glGenTextures(1, &m_perlinTexture);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_uiVertNum, m_uiVertNum, 0, GL_RED, GL_FLOAT, perlinData);

	delete[] perlinData;
}

void Terrain::perlinRings()
{
	// texture mapping heights
	m_vMapTex = glm::vec3(0.0f, 0.55f, 0.05f);
	// create perlin data
	int size = m_uiVertNum * m_uiVertNum;
	float* perlinData = new float[size];
	float scale = (1.0f / m_uiVertNum) * 3;

	float rings = 12.0f;
	float twist = 0.1f; // turbulance strength
	float perlinSize = 32.0f; // size of turbulance

	for (unsigned int x = 0; x < m_uiVertNum; x++)
	{
		for (unsigned int y = 0; y < m_uiVertNum; y++)
		{
			GLfloat xVal = (GLfloat)((x - m_uiVertNum / 2) / m_uiVertNum);
			GLfloat yVal = (GLfloat)((y - m_uiVertNum / 2) / m_uiVertNum);
			GLfloat distVal = glm::sqrt(xVal * xVal + yVal * yVal) + twist * perlinSize / 256.0f;
			GLfloat sineVal = (GLfloat)(128.0 * fabs(sin(2 * rings * distVal * 3.14159)));
			//std::cout << sineVal << std::endl;
			perlinData[y* m_uiVertNum + x] = glm::perlin(glm::vec2(x, y) * sineVal);
		}
	}
	// generate texture
	glGenTextures(1, &m_perlinTexture);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_uiVertNum, m_uiVertNum, 0, GL_RED, GL_FLOAT, perlinData);

	delete[] perlinData;
}

void Terrain::mapSelect()
{
	if (m_iMapIter == 0)
		perlinGeneration();
	else if (m_iMapIter == 1)
		perlinGenNoisy();
	else  if (m_iMapIter == 2)
		perlinRings();

	m_bToggleMap = false;
}
