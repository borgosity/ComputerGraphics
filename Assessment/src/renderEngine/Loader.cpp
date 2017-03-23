#include "Loader.h"



Loader::Loader()
{
	m_vVAOs = {};
	m_vVBOs = {};
	m_vTextures = {};
}


Loader::~Loader()
{
}
/// default VAO loader
RawModel * Loader::loadToVAO(GLfloat positions[], int size)
{
	GLuint vaoID = createVAO();
	createVBO(positions, size);
	storePositionDataInAttributeList(0);
	return new RawModel(vaoID, size/3);
}
///
/// Function to Load data to VAO - 
/// positions = vertex data ( it can contain	- position and colour and texture 
///												- position and colour
///												- position and texture
///												- position ) 
/// posSize = size of vertex data array
/// vertexSize = size of the vertex buffer blocks	- 8(p & c & t)
///													- 6 (p & C) 
///													- 5 (p & t) 
///													- 3 (p)
///
RawModel * Loader::loadToVAO(GLfloat a_positions[], int a_posSize, int a_vertexSize)
{
	GLuint vaoID = createVAO();
	createVBO(a_positions, a_posSize);
	// assume there is always position data
	int startPos = 0;
	// assume there is always position data
	storePositionData(0, a_vertexSize, startPos);
	startPos += 3;
	// if there is more than 1 set of 3 data assume second set is colour
	if (a_vertexSize / 3 >= 2) {
		startPos += 3;
		storeColourData(1, a_vertexSize, startPos);
	}
	// if the data has a remainder of 3 then assume there is tangent data
	if (a_vertexSize / 3 >= 3) {
		startPos += 3;
		storeTangentData(3, a_vertexSize, startPos);
	}
	// if the data has a remainder of 2 then assume there is texture data
	if (a_vertexSize % 3 == 2) {
		storeTextureData(2, a_vertexSize, startPos);
	}
	// unbind vbo and vao
	unbind();
	// return a RawModel object
	return new RawModel(vaoID, a_posSize / a_vertexSize);
}

///
/// Function to Load data to VAO - 
/// positions = vertex data ( it can contain	- position and colour and texture 
///												- position and colour
///												- position and texture
///												- position ) 
/// posSize = size of vertex data array
/// vertexSize = size of the vertex buffer blocks	- 8(p & c & t)
///													- 6 (p & C) 
///													- 5 (p & t) 
///													- 3 (p)
///
RawModel * Loader::loadToVAO(GLfloat a_positions[], int a_posSize, int a_vertexSize, GLuint a_indicies[], int a_indSize)
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
	startPos += 3;
	storePositionData(0, a_vertexSize, startPos);
	// if there is more than 1 set of 3 data assume second set is colour
	if (a_vertexSize / 3 >= 2) {
		startPos += 3;
		storeColourData(1, a_vertexSize, startPos);
	}
	// if the data has a remainder of 3 then assume there is tangent data
	if (a_vertexSize / 3 >= 3) {
		startPos += 3;
		storeTangentData(3, a_vertexSize, startPos);
	}
	// if the data has a remainder of 2 then assume there is texture data
	if (a_vertexSize % 3 == 2) {
		storeTextureData(2, a_vertexSize, startPos);
	}
	// unbind vbo and vao
	unbind();
	// return a RawModel object
	return new RawModel(vaoID, vertCount, hasIndices);
}


/// VAO loader that includes indicies
RawModel * Loader::loadToVAO(GLfloat a_positions[], int a_posSize, GLuint a_indicies[], int a_indSize)
{
	GLuint vaoID = createVAO();
	createVBO(a_positions, a_posSize);			// create and bind VBOs
	bindIndicesBuffer(a_indicies, a_indSize);	// bind indicies
	storePositionDataInAttributeList(0);
	return new RawModel(vaoID, a_indSize);
}

// refactor attempt
// TO BE TESTED
RawModel * Loader::loadToVAO(GLfloat a_positions[], int a_pSize, GLfloat a_colours[], int a_cSize, 
							 GLfloat a_textures[], int a_tSize, GLuint a_indicies[], int a_iSize)
{
	GLuint vaoID = createVAO();
	bindIndicesBuffer(a_indicies, a_iSize);	// bind indicies
	// bind to VBO in the below functions
	storePositionData(0, a_positions, a_pSize);
	storeTextureData(1, a_colours, a_cSize);
	storeTextureData(2, a_textures, a_tSize);
	// unbind vbo and vao
	unbind();
	return new RawModel(vaoID, a_iSize / sizeof(GLuint));
}

RawModel * Loader::loadTextureVAO(GLfloat a_positions[], int a_size, GLuint a_indicies[], int a_indSize)
{
	GLuint vaoID = createVAO();
	bindIndicesBuffer(a_indicies, a_indSize);	// bind indicies
	createVBO(a_positions, a_size);
	storeTextureDataInAttributeList(0);

	return new RawModel(vaoID, a_indSize / sizeof(GLuint));
}

/* 
!!!!--- this function has been moved to the Texture class
*/
GLuint Loader::loadTexture(std::string a_fileName)
{
	int width, height;
	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	// bind texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	// All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// read in the texture file
	unsigned char* image = SOIL_load_image(a_fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	// generate texture from the image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	// clear image memory and unbind the texture
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return textureID;
}

GLuint Loader::createVAO()
{
	GLuint vaoID = 0;
	glGenVertexArrays(1, &vaoID);
	m_vVAOs.push_back(vaoID);
	// Bind the Vertex Array Object first
	glBindVertexArray(vaoID);

	return vaoID;
}

void Loader::createVBO(GLfloat data[], int size)
{
	GLuint vboID = 0;
	// bind and set vertex buffer(s) and attribute pointer(s).
	glGenBuffers(1, &vboID);
	m_vVBOs.push_back(vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
// template was made for this function
//void Loader::createVBO(Vertex * data, int size)
//{
//	GLuint vboID = 0;
//	// bind and set vertex buffer(s) and attribute pointer(s).
//	glGenBuffers(1, &vboID);
//	m_vVBOs.push_back(vboID);
//	glBindBuffer(GL_ARRAY_BUFFER, vboID);
//	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
//}

void Loader::bindIndicesBuffer(GLuint indices[], int size)
{
	GLuint eboID = 0;
	// bind and set indicies buffer.
	glGenBuffers(1, &eboID);
	m_vVBOs.push_back(eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++
// OLD CODE CAN BE REMOVED LATER
void Loader::storePositionDataInAttributeList(int attributeNumber)
{
	// Position attribute
	glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(attributeNumber);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind current VBO
	glBindVertexArray(0);			  // unbind current VAO
}

void Loader::storeColourDataInAttributeList(int attributeNumber)
{
	// Position attribute
	glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(attributeNumber);
	// Color attribute
	glVertexAttribPointer(attributeNumber + 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(attributeNumber + 1);
	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind current VBO
	glBindVertexArray(0);			  // unbind current VAO
}

void Loader::storeTextureDataInAttributeList(int attributeNumber)
{
	// Position attribute
	glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(attributeNumber);
	// Color attribute
	glVertexAttribPointer(attributeNumber + 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(attributeNumber + 1);
	// Texture attribute
	glVertexAttribPointer(attributeNumber + 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind current VBO
	glBindVertexArray(0);			  // unbind current VAO
}
//   END OLD CODE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*****************************************************************************************
	Single VBO with a single array of data functions 
	
	# TODO # adjust vertex data size to not be fixed pos and colour can be 3 or 4 which will also change start pos

*******************************************************************************************/
void Loader::storePositionData(int attributeNumber, int vertSize, int startPos)
{
	// Position attribute
	glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, vertSize * sizeof(GLfloat), (GLvoid*)startPos);
	glEnableVertexAttribArray(attributeNumber);
}

void Loader::storeColourData(int attributeNumber, int vertSize, int startPos)
{
	// Color attribute
	glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, vertSize * sizeof(GLfloat), (GLvoid*)(startPos * sizeof(GLfloat)));
	glEnableVertexAttribArray(attributeNumber);
}

void Loader::storeTextureData(int attributeNumber, int vertSize, int startPos)
{
	// Texture attribute
	glVertexAttribPointer(attributeNumber, 2, GL_FLOAT, GL_FALSE, vertSize * sizeof(GLfloat), (GLvoid*)(startPos * sizeof(GLfloat)));
	glEnableVertexAttribArray(attributeNumber);
}
void Loader::storeTangentData(int attributeNumber, int vertSize, int startPos)
{
	// Texture attribute
	glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, vertSize * sizeof(GLfloat), (GLvoid*)(startPos * sizeof(GLfloat)));
	glEnableVertexAttribArray(attributeNumber);
}
/*****************************************************************************************
	- ## NOT TESTED ## - Indiviual VBOs for each individual data array functions 
*******************************************************************************************/
void Loader::storePositionData(int attributeNumber, GLfloat data[], int size)
{
	// create VBO with data
	createVBO(data, size);	
	// Position attribute
	glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attributeNumber);
}

void Loader::storeColourData(int attributeNumber, GLfloat data[], int size)
{
	// create VBO with data
	createVBO(data, size);
	// Color attribute
	glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attributeNumber);
}

void Loader::storeTextureData(int attributeNumber, GLfloat data[], int size)
{
	// create VBO with data
	createVBO(data, size);
	// Texture attribute
	glVertexAttribPointer(attributeNumber, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attributeNumber);
}

void Loader::unbind()
{
	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind current VBO
	glBindVertexArray(0);			  // unbind current VAO
}



void Loader::cleanUp()
{
	// clean up VAOs
	for each (auto var in m_vVAOs)
	{
		glDeleteVertexArrays(1, &var);
	}
	// clean up VBOs
	for each (auto var in m_vVBOs)
	{
		glDeleteBuffers(1, &var);
	}
	// clean up textures
	for each (auto var in m_vTextures)
	{
		glDeleteTextures(1, &var);
	}
}

