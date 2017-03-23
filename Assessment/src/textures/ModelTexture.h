#pragma once
#include <glew.h>

class ModelTexture
{
public:
	ModelTexture(unsigned int a_id);
	~ModelTexture();

	GLuint textureID() { return m_uiTextureID; }

private:
	GLuint m_uiTextureID;
};

