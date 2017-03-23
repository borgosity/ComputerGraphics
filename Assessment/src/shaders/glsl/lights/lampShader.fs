// shadertype = glsl
#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform vec3 lampColor;
uniform vec3 lightColor;

void main()
{    
    color = vec4(texture(texture_diffuse1, TexCoords)) * vec4(lampColor, 1.0f);
}
