#pragma once


#include "MeshModel.h"

class AnimatedModel
{
public:
	AnimatedModel(std::string a_filePath1, std::string a_filePath2);
	~AnimatedModel();

	glm::mat4 transform;
	void animate(ShaderProgram & a_shaderProgram);

private:
	MeshCoords * m_startModel = nullptr;
	MeshCoords * m_endModel = nullptr;

	std::vector<Mesh> m_vMeshes;
	std::vector<MeshTexture> m_vMeshTextures;
	std::string m_sDirectory;

	// load model
	void loadModels(std::string a_filePath1, std::string a_filePath2);

	// process ASSIMP's root node
	void processRootNodes(aiNode & a_node, const aiScene & a_scene1, aiNode & a_node2, const aiScene & a_scene2);
	// process models mesh
	MeshCoords * processMesh(aiMesh & a_mesh, const aiScene & a_scene);
	
	
	// load materials textures
	std::vector<MeshTexture> loadMaterialTextures(aiMaterial & a_material, aiTextureType a_texType, std::string a_typeName);
	// load texture from file
	GLint loadTexture(std::string a_path, std::string a_directory);

};

