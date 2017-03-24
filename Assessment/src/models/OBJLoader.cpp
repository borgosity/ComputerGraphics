#include "OBJLoader.h"



OBJLoader::OBJLoader()
{
}


OBJLoader::~OBJLoader()
{
}

//void OBJLoader::loadObjModel(std::string a_filePath, RawModel & a_rawModel, Loader & a_loader)
//{	
//	// file loading variables
//	tinyobj::attrib_t attribs;
//	std::vector<tinyobj::shape_t> shapes;
//	std::vector<tinyobj::material_t> materials;
//	std::string err;
//	// load file and get result
//	bool success = tinyobj::LoadObj(&attribs, &shapes, &materials, &err, a_filePath.c_str());
//
//	if (shapes.size() > 1) {
//		std::cout << " LOAD MODEL ERROR -> model has more than one shape in it, use function that supports multiple shapes!" << std::endl;
//		return;
//	}
//
//	// collect triangle vertices
//	auto shape = shapes[0];
//	int vertSize = shape.mesh.num_face_vertices.size();
//	ObjVertex * vertices = new ObjVertex[vertSize];
//	int index = 0;
//	int vertIndex = 0;
//	for (auto face : shape.mesh.num_face_vertices)
//	{
//		// each face has vertices
//		for (int i = 0; i < 3; ++i) {
//			tinyobj::index_t idx = shape.mesh.indices[index + i];
//			ObjVertex v = { };
//			// positions
//			vertices[index + i].position = glm::vec3(
//				(GLfloat)attribs.vertices[3 * idx.vertex_index + 0],
//				(GLfloat)attribs.vertices[3 * idx.vertex_index + 1],
//				(GLfloat)attribs.vertices[3 * idx.vertex_index + 2]
//			);
//			// normals
//			if (attribs.normals.size() > 0) {
//				vertices[index + i].normal = glm::vec3(
//					(GLfloat)attribs.normals[3 * idx.normal_index + 0],
//					(GLfloat)attribs.normals[3 * idx.normal_index + 1],
//					(GLfloat)attribs.normals[3 * idx.normal_index + 2]
//				);
//			}
//			// texture coordinates
//			if (attribs.texcoords.size() > 0) {
//				vertices[index + i].uv = glm::vec2(
//					(GLfloat)attribs.texcoords[2 * idx.texcoord_index + 0],
//					(GLfloat)attribs.texcoords[2 * idx.texcoord_index + 1]
//				);
//			}
//		}
//		index += face;
//	}
//	// load vertices to VAO and create a raw model
//	//a_rawModel = a_loader.loadToVAO(vertices, vertSize * sizeof(ObjVertex), 7, nullptr, 0);
//	// clean up
//	delete[] vertices;
//}

void OBJLoader::loadObjModel(std::string a_filePath, RawModel * a_rawModel, Loader & a_loader)
{
	// file loading variables
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	// load file and get result
	bool success = tinyobj::LoadObj(&attribs, &shapes, &materials, &err, a_filePath.c_str());

	// === ERROR CHECK file loading ===
	// single shape model function error
	if (shapes.size() > 1) {
		std::cout << " LOAD MODEL ERROR -> model has more than one shape in it, use function that supports multiple shapes!" << std::endl;
		return;
	}
	// was file found
	if (shapes.size() == 0) {
		std::cout << " LOAD MODEL ERROR -> " << err << std::endl;
		return;
	}

	// initialise some variables for retrieving vertices 
	auto shape = shapes._Myfirst();
	std::vector<Vertex3> vertices;
	int index = 0;

	// loop though faces and store vertex data
	for (auto face : shape->mesh.num_face_vertices) {
		for (int i = 0; i < 3; ++i) {
			tinyobj::index_t idx = shape->mesh.indices[index + i];
			Vertex3 v = { 0 };
			// positions
			v.x = attribs.vertices[3 * idx.vertex_index + 0];
			v.y = attribs.vertices[3 * idx.vertex_index + 1];
			v.z = attribs.vertices[3 * idx.vertex_index + 2];
			// normals
			if (attribs.normals.size() > 0) {
				v.nx = attribs.normals[3 * idx.normal_index + 0];
				v.ny = attribs.normals[3 * idx.normal_index + 1];
				v.nz = attribs.normals[3 * idx.normal_index + 2];
			}
			// texture coordinates
			if (attribs.texcoords.size() > 0) {
				v.u = attribs.texcoords[2 * idx.texcoord_index + 0];
				v.v = attribs.texcoords[2 * idx.texcoord_index + 1];
			}
			vertices.push_back(v);
		}
		index += face;
	}

	// count vert sizes
	int vertexSize = 3;
	if (attribs.normals.size() > 0) vertexSize += 3;
	if (attribs.texcoords.size() > 0) vertexSize += 2;

	// create raw model from vertex data, there are no indicies so passing nullptr and 0
	a_rawModel = a_loader.loadToVAO(vertices.data(), vertices.size() * sizeof(Vertex3), vertexSize, nullptr, 0);
}

void OBJLoader::loadObjWithMaterial(std::string a_filePath, std::string a_materialDir, RawModel & a_rawModel, Loader & a_loader)
{
	
}
