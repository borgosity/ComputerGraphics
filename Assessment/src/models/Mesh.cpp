#include "Mesh.h"


Mesh::Mesh(std::vector<Vertex> a_vertices, std::vector<GLuint> a_indices, std::vector<MeshTexture> a_textures, float a_shininess)
{
	m_vVertices = a_vertices;
	m_vIndices = a_indices;
	m_vTextures = a_textures;
	a_shininess == 0.0f ? m_fShininess = 32.0f : m_fShininess = a_shininess;
	// Now that we have all the required data, set the vertex buffers and its attribute pointers.
	setupMesh();
}

Mesh::Mesh(MeshCoords * a_startMesh, MeshCoords * a_endMesh)
{
	m_vVertices = a_startMesh->vertices;
	m_vIndices = a_startMesh->indices;
	m_vTextures = a_startMesh->textures;
	m_startMesh = a_startMesh;
	m_endMesh = a_endMesh;

	//int count = 0;
	//for (int i = 0; i < m_startMesh->vertices.size(); i++)
	//{
	//	if (m_startMesh->vertices[i].position.x != m_endMesh->vertices[i].position.x &&
	//		m_startMesh->vertices[i].position.y != m_endMesh->vertices[i].position.y &&
	//		m_startMesh->vertices[i].position.z != m_endMesh->vertices[i].position.z) {
	//		std::cout << "difference" << std::endl;
	//	}
	//	count++;
	//}

	a_startMesh->shininess == 0.0f ? m_fShininess = 32.0f : m_fShininess = a_startMesh->shininess;
	mergeMeshData();
}

Mesh::~Mesh()
{
}

void Mesh::draw(ShaderProgram & a_shaderProgram)
{
	// Bind appropriate textures
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;
	GLuint emissiveNr = 1;
	for (GLuint i = 0; i < m_vTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
										  // Retrieve texture number (the N in diffuse_textureN)
		std::stringstream ss;
		std::string number;
		std::string name = m_vTextures[i].type;
		int hasEmissive = 0; // set emissive to false

		if (name == "texture_diffuse") {
			ss << diffuseNr++; // Transfer GLuint to stream
		}
		else if (name == "texture_specular") {
			ss << specularNr++; // Transfer GLuint to stream
		}
		else if (name == "texture_normal") {
			ss << normalNr++; // Transfer GLuint to stream
		}
		else if (name == "texture_emissive") {
			ss << emissiveNr++; // Transfer GLuint to stream
			hasEmissive = 1;	// set emissive to true
		}
		number = ss.str();

		// Now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(a_shaderProgram.ID(), ("material." + name + number).c_str()), i);
		glUniform1i(glGetUniformLocation(a_shaderProgram.ID(), "hasEmissive"), hasEmissive);
// ## DEBUG ##
		//std::cout << ("material." + name + number).c_str() << std::endl;
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, m_vTextures[i].id);
	}

	// Also set each mesh's shininess property to a default value 
	// - (if you want you could extend this to another mesh property and possibly change this value)
	glUniform1f(glGetUniformLocation(a_shaderProgram.ID(), "material.shininess"), m_fShininess);

	// Draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_vIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Always good practice to set everything back to defaults once configured.
	for (GLuint i = 0; i < m_vTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::animate(ShaderProgram & a_shaderProgram)
{
	// Bind appropriate textures
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;
	GLuint emissiveNr = 1;
	for (GLuint i = 0; i < m_vTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
										  // Retrieve texture number (the N in diffuse_textureN)
		std::stringstream ss;
		std::string number;
		std::string name = m_vTextures[i].type;
		int hasEmissive = 0; // set emissive to false

		if (name == "texture_diffuse") {
			ss << diffuseNr++; // Transfer GLuint to stream
		}
		else if (name == "texture_specular") {
			ss << specularNr++; // Transfer GLuint to stream
		}
		else if (name == "texture_normal") {
			ss << normalNr++; // Transfer GLuint to stream
		}
		else if (name == "texture_emissive") {
			ss << emissiveNr++; // Transfer GLuint to stream
			hasEmissive = 1;	// set emissive to true
		}
		number = ss.str();

		// Now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(a_shaderProgram.ID(), ("material." + name + number).c_str()), i);
		glUniform1i(glGetUniformLocation(a_shaderProgram.ID(), "hasEmissive"), hasEmissive);
		// ## DEBUG ##
		//std::cout << ("material." + name + number).c_str() << std::endl;
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, m_vTextures[i].id);
	}

	// Also set each mesh's shininess property to a default value 
	// - (if you want you could extend this to another mesh property and possibly change this value)
	glUniform1f(glGetUniformLocation(a_shaderProgram.ID(), "material.shininess"), m_fShininess);

	// Draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_vTextures.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Always good practice to set everything back to defaults once configured.
	for (GLuint i = 0; i < m_vTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::setupMesh()
{
	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &EBO1);
	// bind VAO
	glBindVertexArray(VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, m_vVertices.size() * sizeof(Vertex), &m_vVertices[0], GL_STATIC_DRAW);
	// bind and set indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vIndices.size() * sizeof(GLuint), &m_vIndices[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));
	// cleanup
	glBindVertexArray(0);
}

void Mesh::mergeMeshData()
{
	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO1);
	glGenBuffers(1, &EBO2);
	// bind VAO
	glBindVertexArray(VAO);

	// -------- start mmesh ---------------------------------------------------------
	// Load data into vertex buffers for animation start mesh
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, m_startMesh->vertices.size() * sizeof(Vertex), &m_startMesh->vertices[0], GL_STATIC_DRAW);
	// bind and set indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_startMesh->indices.size() * sizeof(GLuint), &m_startMesh->indices[0], GL_STATIC_DRAW);
	// Set the vertex attribute pointers
	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	// Vertex Texture Coords --> shared between both meshes
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

	// -------- end mesh --------------------------------------------------------------
	// Load data into vertex buffers for animation end mesh
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, m_endMesh->vertices.size() * sizeof(Vertex), &m_endMesh->vertices[0], GL_STATIC_DRAW);
	// bind and set indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_endMesh->indices.size() * sizeof(GLuint), &m_endMesh->indices[0], GL_STATIC_DRAW);
	// Vertex Positions
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	// cleanup
	glBindVertexArray(0);
}
