// shadertype = glsl
#version 410 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec4 colour;

out VS_OUT{
	vec4 Position;
	vec3 Normal;
	vec2 TexCoord;
	vec4 Colour;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float keyTime;

void main()
{
    vs_out.Position = position;
    vs_out.Normal = normal;
	vs_out.TexCoord = texCoord;
	vs_out.Colour = colour;
	    
	gl_Position = projection * view * /*model * */position;
}
