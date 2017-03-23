#include "AnimatedModel.h"



AnimatedModel::AnimatedModel(std::string a_filePath1, std::string a_filePath2)
{
	transform = glm::mat4(1.0f);
	loadModels(a_filePath1, a_filePath2);
}

AnimatedModel::~AnimatedModel()
{
	delete m_startModel;
	m_startModel = nullptr;
	delete m_endModel;
	m_endModel = nullptr;
}

void AnimatedModel::animate(ShaderProgram & a_shaderProgram)
{
	a_shaderProgram.uniformMat4("model", transform);
	// draw model meshes
	for (GLuint i = 0; i < m_vMeshes.size(); i++)
		m_vMeshes[i].draw(a_shaderProgram);
}

void AnimatedModel::loadModels(std::string a_filePath1, std::string a_filePath2)
{
	// Read file using ASSIMP
	Assimp::Importer importer1;
	Assimp::Importer importer2;
	const aiScene* scene1 = importer1.ReadFile(a_filePath1, aiProcess_Triangulate | aiProcess_FlipUVs);
	const aiScene* scene2 = importer2.ReadFile(a_filePath2, aiProcess_Triangulate | aiProcess_FlipUVs);


	// Error Check
	if ((!scene1 || scene1->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene1->mRootNode) ||
		(!scene2 || scene2->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene2->mRootNode))  // if is Not Zero
	{
		std::cout << "ERROR -> ASSIMP Animate - " << importer1.GetErrorString() << std::endl;
		std::cout << "ERROR -> ASSIMP Animate - " << importer2.GetErrorString() << std::endl;
		return;
	}
	// Retrieve the directory path of the filepath
	m_sDirectory = a_filePath1.substr(0, a_filePath1.find_last_of('/'));

	// Process ASSIMP's root node recursively
	processRootNodes(*scene1->mRootNode, *scene1, *scene2->mRootNode, *scene2);
}

void AnimatedModel::processRootNodes(aiNode & a_node1, const aiScene & a_scene1, aiNode & a_node2, const aiScene & a_scene2)
{
	
	// Process each mesh located at the current node
	for (GLuint i = 0; i < a_node1.mNumMeshes; i++)
	{
		// The node object only contains indices to index the actual objects in the scene. 
		// The scene contains all the data.
		aiMesh* mesh1 = a_scene1.mMeshes[a_node1.mMeshes[i]];
		aiMesh* mesh2 = a_scene2.mMeshes[a_node2.mMeshes[i]];
		m_startModel = processMesh(*mesh1, a_scene1);
		m_endModel = processMesh(*mesh2, a_scene2);

		m_vMeshes.push_back(Mesh(m_startModel, m_endModel));
	}
	// process children nodes if any
	for (GLuint i = 0; i < a_node1.mNumChildren; i++)
	{
		processRootNodes(*a_node1.mChildren[i], a_scene1, *a_node2.mChildren[i], a_scene2);
	}
}

MeshCoords * AnimatedModel::processMesh(aiMesh & a_mesh, const aiScene & a_scene)
{
	// Data vectors
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<MeshTexture> textures;
	GLfloat shininess = 0.0f;

	// Get mesh's vertices
	for (GLuint i = 0; i < a_mesh.mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // used to convert assimps vector to glm's vec3 format
						  // Positions
		vector.x = a_mesh.mVertices[i].x;
		vector.y = a_mesh.mVertices[i].y;
		vector.z = a_mesh.mVertices[i].z;
		vertex.position = vector;
		// Normals
		vector.x = a_mesh.mNormals[i].x;
		vector.y = a_mesh.mNormals[i].y;
		vector.z = a_mesh.mNormals[i].z;
		vertex.normal = vector;
		// Texture Coordinates if any
		if (a_mesh.mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = a_mesh.mTextureCoords[0][i].x;
			vec.y = a_mesh.mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else {
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	// Go through all faces and get vertex indices.
	for (GLuint i = 0; i < a_mesh.mNumFaces; i++)
	{
		aiFace face = a_mesh.mFaces[i];
		// Get face indicies and add to indices vector
		for (GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	// Process materials
	if (a_mesh.mMaterialIndex >= 0)
	{
		aiMaterial* material = a_scene.mMaterials[a_mesh.mMaterialIndex];
		// Shader uniform texture naming convention <type>Texture<number>:
		// Diffuse: diffuseTexture1
		// Specular: specularTexture1
		// Normal: normalTexture1
		// Emissive: emissiveTexture1

		// Diffuse maps
		std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(*material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// Specular maps
		std::vector<MeshTexture> specularMaps = loadMaterialTextures(*material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// Normal maps
		std::vector<MeshTexture> normalMaps = loadMaterialTextures(*material, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// Emmisive maps
		std::vector<MeshTexture> emissiveMaps = loadMaterialTextures(*material, aiTextureType_EMISSIVE, "texture_emissive");
		textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());

		unsigned int max;
		aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shininess, &max);

	}

	// Return a mesh object created from the extracted mesh data
	return new MeshCoords{ vertices, indices, textures, shininess };
}

std::vector<MeshTexture> AnimatedModel::loadMaterialTextures(aiMaterial & a_material, aiTextureType a_texType, std::string a_typeName)
{
	std::vector<MeshTexture> textures;
	for (GLuint i = 0; i < a_material.GetTextureCount(a_texType); i++)
	{
		aiString str;
		a_material.GetTexture(a_texType, i, &str);
		// Check if texture was already loaded, skip to next if true
		GLboolean skip = false;
		for (GLuint j = 0; j < m_vMeshTextures.size(); j++)
		{
			if (std::strcmp(m_vMeshTextures[j].path.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(m_vMeshTextures[j]);
				skip = true; // texture already loaded
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			MeshTexture texture;
			texture.id = loadTexture(str.C_Str(), m_sDirectory);
			texture.type = a_typeName;
			texture.path = str;
			textures.push_back(texture);
			m_vMeshTextures.push_back(texture);  // store texture
		}
	}
	return textures;
}

GLint AnimatedModel::loadTexture(std::string a_path, std::string a_directory)
{
	//Generate texture ID and load texture data 
	std::string filename = a_path;
	filename = a_directory + '/' + filename;
	std::ifstream file(filename);
	// check if file is valid before proceeding
	if (file.good()) {
		GLuint textureID;
		glGenTextures(1, &textureID);
		int width, height;
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
		return textureID;
	}
	else {
		std::cout << "ERROR -> Failure reading texture file: " << std::endl;
		filename = "file not found";
		return 0;
	}
}
