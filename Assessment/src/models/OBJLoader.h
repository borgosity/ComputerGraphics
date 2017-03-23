#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <glew.h>
#include <tiny_obj_loader.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

// source includes
#include "RawModel.h"
#include "Loader.h"

class OBJLoader
{
public:
	OBJLoader();
	~OBJLoader();

	static void loadObjModel(std::string a_filePath, RawModel & a_rawModel, Loader & a_loader );
	static void loadObjWithMaterial(std::string a_filePath, std::string a_materialDir, RawModel & a_rawModel, Loader & a_loader);
};

