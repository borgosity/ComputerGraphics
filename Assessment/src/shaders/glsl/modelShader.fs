// shadertype = glsl
#version 330 core
in vec3 ourColour;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

out vec4 color;

// light
uniform vec3 lightDirection;

// Texture samplers
uniform sampler2D diffuse;
uniform sampler2D normal;

void main()
{
	// tangent, bitangent, normal - transform to world space for calculation
	mat3 TBN = mat3(normalize(vTangent), normalize(vBiTangent), normalize(ourColour));
	
	vec3 N = texture(normal, vTexCoord).xyz * 2 -1;
	
	float d = max(0, dot(normalize(TBN * N), normalize(lightDirection)));

	color = texture(diffuse, vTexCoord);
	color.rgb = color.rgb * d;
	//color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);

    //color = texture(ourTexture1, TexCoord) * vec4(ourColor, 1.0f);
	//color = vec4(ourColor, 1.0f);
}
