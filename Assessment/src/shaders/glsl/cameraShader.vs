// shadertype = glsl
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 vPosition;
out vec3 vNormal;
out vec2 TexCoord;

// camera
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
	vPosition = position;
    vNormal = color;
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}
