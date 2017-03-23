#pragma once
#include "ShaderProgram.h"
class TextureShader :
	public ShaderProgram
{
public:
	TextureShader();
	virtual ~TextureShader();
	virtual void bindAttributes();
};

