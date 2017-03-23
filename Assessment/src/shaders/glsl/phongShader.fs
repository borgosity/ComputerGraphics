// shadertype = glsl
#version 410 core
in vec2 vTexCoords;		// texture coordinates
in vec3 vSurfaceNorm;	// surface normals
in vec3 vToCamera;		// vector to the camera positionout vec4 color;
in vec3 vToLight;		// vector to the light source

out vec4 colour;		// final output colour

// light uniforms
uniform vec3 lightColour;
uniform float shineDamper; // object shininess
uniform float reflectivity; // object reflection

// Texture samplers
uniform sampler2D modelTexture;
uniform sampler2D normalTex;

void main()
{
	// nomalise values
	vec3 unitNormal = normalize(vSurfaceNorm);
	vec3 unitToLight = normalize(vToLight);
	// diffuse brightness with light colour
	float brightness = max(dot(unitNormal, unitToLight), 0.0 );
	vec3 diffuse = brightness * lightColour;
	// calculate reflected light direction
	vec3 unitToCamera = normalize(vToCamera);
	vec3 lightDirection = -unitToLight;
	vec3 reflectedLight = reflect(lightDirection, unitNormal);
	// calculate specular light
	float specularFactor = max(dot(reflectedLight, unitToCamera), 0.0);
	float dampedFactor = pow(specularFactor, shineDamper);
	vec3 finalSpecular = dampedFactor * reflectivity * lightColour;
	// final output colour
	colour =  vec4(diffuse, 1.0f) * texture(modelTexture, vTexCoords) + vec4(finalSpecular, 1.0);

}
