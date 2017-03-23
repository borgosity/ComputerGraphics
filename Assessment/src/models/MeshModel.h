#pragma once
// std includes
#include <map>
#include <vector>
#include <fstream>
// GL includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// source includes
#include "Mesh.h"

class MeshModel
{
public:
	MeshModel(std::string a_filePath);
	~MeshModel();

	glm::mat4 transform;
	void draw(ShaderProgram & a_shaderProgram);
	void addTexture(std::string a_fileName);

private:
	std::vector<Mesh> m_vMeshes;
	std::vector<MeshTexture> m_vMeshTextures;
	std::string m_sDirectory;

	// load model
	void loadModel(std::string a_filePath);

	// process ASSIMP's root node
	void processRootNode(aiNode & a_node, const aiScene & a_scene);
	// process models mesh
	Mesh processMesh(aiMesh & a_mesh, const aiScene & a_scene);
	// load materials textures
	std::vector<MeshTexture> loadMaterialTextures(aiMaterial & a_material, aiTextureType a_texType, std::string a_typeName);
	// load texture from file
	GLint loadTexture(std::string a_path, std::string a_directory);
};

