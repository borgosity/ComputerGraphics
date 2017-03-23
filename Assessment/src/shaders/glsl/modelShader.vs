// shadertype = glsl
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;

out vec3 ourColour;
out vec2 vTexCoord;
out vec3 vTangent;
out vec3 vBiTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    ourColour = color;
    vTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
	vTangent = tangent;
	vBiTangent = cross(ourColour, vTangent);
}
