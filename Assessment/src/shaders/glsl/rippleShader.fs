// shadertype = glsl
#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform vec4 uniformColour;

// Texture samplers
uniform sampler2D diffuse;
uniform sampler2D normal;

void main()
{
	// Linearly interpolate between both textures (second texture is only slightly combined)
	color = mix(texture(diffuse, TexCoord), texture(normal, TexCoord), 0.2);
    //color = texture(diffuse, TexCoord) * vec4(ourColor, 1.0f);

}
