#include "TextureShader.h"

// Texture Shader files
const GLchar * TEXTURE_VERTEX_FILE = "src/shaders/textureShader.vs";
const GLchar * TEXTURE_FRAGMENT_FILE = "src/shaders/textureShader.fs";


TextureShader::TextureShader() : ShaderProgram(TEXTURE_VERTEX_FILE, TEXTURE_FRAGMENT_FILE)
{
}


TextureShader::~TextureShader()
{
}

void TextureShader::bindAttributes()
{
	//bindAttribute(0, "position");	// bind the 'position' variable to position 0
	//bindAttribute(1, "color");		// bind the 'color' variable to position 1
}
