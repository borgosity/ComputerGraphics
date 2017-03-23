#include "Shaders.h"

 ShaderPath Shader::cameraShader =		{ "src/shaders/glsl/cameraShader.vs",		"src/shaders/glsl/cameraShader.fs" };
 ShaderPath Shader::colourShader =		{ "src/shaders/glsl/colourShader.vs",		"src/shaders/glsl/colourShader.fs" };
 ShaderPath Shader::dualTextureShader = { "src/shaders/glsl/dualTextureShader.vs",	"src/shaders/glsl/dualTextureShader.fs" };
 ShaderPath Shader::indexShader =		{ "src/shaders/glsl/indexShader.vs",		"src/shaders/glsl/indexShader.fs" };
 // lights
 ShaderPath Shader::lampShader =	{ "src/shaders/glsl/lights/lampShader.vs",		"src/shaders/glsl/lights/lampShader.fs" };
 ShaderPath Shader::lightShader =	{ "src/shaders/glsl/lights/lightingShader.vs",	"src/shaders/glsl/lights/lightingShader.fs" };
 ShaderPath Shader::pointLight =	{ "src/shaders/glsl/lights/pointLight.vs",		"src/shaders/glsl/lights/pointLight.fs" };
 ShaderPath Shader::spotLight =		{ "src/shaders/glsl/lights/spotLight.vs",	"src/shaders/glsl/lights/spotLight.fs" };
 ShaderPath Shader::spotLight_soft = { "src/shaders/glsl/lights/spotLight_soft.vs",	"src/shaders/glsl/lights/spotLight_soft.fs" };
 // particles
 ShaderPath Shader::particleShader = { "src/shaders/glsl/particles/particleShader.vs", "src/shaders/glsl/particles/particleShader.fs" };
 ShaderPath Shader::planeShader  = { "src/shaders/glsl/planeShader.vs",		"src/shaders/glsl/planeShader.fs" };
 ShaderPath Shader::terrainShader = { "src/shaders/glsl/terrainShader.vs",		"src/shaders/glsl/terrainShader.fs" };

 ShaderPath Shader::animeShader = { "src/shaders/glsl/animationShader.vs",		"src/shaders/glsl/animationShader.fs" };
 ShaderPath Shader::meshShader =	{ "src/shaders/glsl/meshShader.vs",		"src/shaders/glsl/meshShader.fs" };
 ShaderPath Shader::modelShader =	{ "src/shaders/glsl/modelShader.vs",	"src/shaders/glsl/modelShader.fs" };
 
 
 ShaderPath Shader::phongShader =	{ "src/shaders/glsl/phongShader.vs",	"src/shaders/glsl/phongShader.fs" };
 ShaderPath Shader::rippleShader =	{ "src/shaders/glsl/rippleShader.vs",	"src/shaders/glsl/rippleShader.fs" };
 ShaderPath Shader::staticShader =	{ "src/shaders/glsl/staticShader.vs",	"src/shaders/glsl/staticShader.fs" };
 ShaderPath Shader::textureShader =		{ "src/shaders/glsl/textureShader.vs",	 "src/shaders/glsl/textureShader.fs" };
 ShaderPath Shader::transformShader =	{ "src/shaders/glsl/transformShader.vs", "src/shaders/glsl/transformShader.fs" };
 ShaderPath Shader::uniformShader =		{ "src/shaders/glsl/uniformShader.vs",	 "src/shaders/glsl/uniformShader.fs" };