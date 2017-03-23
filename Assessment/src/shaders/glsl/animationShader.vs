// shadertype = glsl
#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 position2;
layout (location = 4) in vec3 normal2;

out VS_OUT{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoord;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float keyTime;

void main()
{
	vec3 lerpedPosition = mix(position, position2, keyTime);
	vec3 lerpedNormal = mix(normal, normal2, keyTime);

    vs_out.Position = lerpedPosition;
    vs_out.Normal = lerpedNormal;
	vs_out.TexCoord = texCoord;

	    
	gl_Position = projection * view * model * vec4(lerpedPosition, 1.0f);
}
