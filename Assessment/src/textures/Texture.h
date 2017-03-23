#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <glew.h>
#include <SOIL.h>

class Texture
{
public:
	Texture();
	Texture(const char * a_filename);
	~Texture();

	void load(const char * a_filename);
	GLuint ID() { return m_uiID; }
	GLuint width() { return m_uiWidth; }
	GLuint height() { return m_uiHeight; }
	std::string fileName() { return m_sFilename; }

private:
	std::string	m_sFilename;
	GLuint		m_uiWidth;
	GLuint		m_uiHeight;
	GLuint		m_uiID;
};

