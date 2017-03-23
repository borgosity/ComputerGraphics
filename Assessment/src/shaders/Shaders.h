
#pragma once
#include <string>
#include <glew.h>

struct ShaderPath
{
	std::string vertexShader;
	std::string fragmentShader;
};

struct Shader
{
	static ShaderPath cameraShader;
	static ShaderPath colourShader;
	static ShaderPath dualTextureShader;
	static ShaderPath indexShader;
	// lights
	static ShaderPath lampShader;
	static ShaderPath lightShader;
	static ShaderPath pointLight;
	static ShaderPath spotLight;
	static ShaderPath spotLight_soft;
	// particles
	static ShaderPath particleShader;

	static ShaderPath animeShader;
	static ShaderPath modelShader;
	static ShaderPath meshShader;

	static ShaderPath planeShader;
	static ShaderPath terrainShader;

	static ShaderPath phongShader;
	static ShaderPath rippleShader;

	static ShaderPath staticShader;
	static ShaderPath textureShader;
	static ShaderPath transformShader;
	static ShaderPath uniformShader;
};




