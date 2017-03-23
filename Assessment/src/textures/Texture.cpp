#include "Texture.h"



Texture::Texture()
{
	m_uiWidth = 0;
	m_uiHeight = 0;
	m_sFilename = "none";
	m_uiID = 0;
}

Texture::Texture(const char * a_fileName)
{
	m_uiWidth = 0;
	m_uiHeight = 0;
	m_sFilename = "none";
	m_uiID = 0;
	load(a_fileName);
}


Texture::~Texture()
{
}

void Texture::load(const char * a_fileName)
{
	std::ifstream file(a_fileName);

	if (file.good()) {
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
		unsigned char* image = SOIL_load_image(a_fileName, &width, &height, 0, SOIL_LOAD_RGB);
		// generate texture from the image
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		// clear image memory and unbind the texture
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_sFilename = a_fileName;
		m_uiWidth = width;
		m_uiHeight = height;
		m_uiID = textureID;
	}
	else {
		std::cout << "ERROR -> Failure reading texture file: " << a_fileName << std::endl;
		m_sFilename = "file not found";
	}
}
