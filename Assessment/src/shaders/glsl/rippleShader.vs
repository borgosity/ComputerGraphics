// shadertype = glsl
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// effects uniforms
uniform float time;
uniform float heightScale;

void main()
{
    ourColor = color;
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
	// ripple effect
	vec4 pos = vec4(position, 1.0f);
	pos.y += sin(time + position.x) * heightScale;
	// set position
    gl_Position = projection * view * model * pos;
}
