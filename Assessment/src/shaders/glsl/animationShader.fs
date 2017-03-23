// shadertype = glsl
#version 410 core

in VS_OUT {
	vec3 Position;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;

out vec4 fragColor;

// light
uniform vec3 lightPos;
uniform vec4 specColour;
uniform vec3 viewPos;
uniform bool blinn;

// Texture samplers
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_emissive;

void main()
{
    // set a default color to catch bugs
	vec3 color = vec3(0.1f, 0.2f, 0.3f);
	//vec3 red = vec3(0.9f, 0.5f, 0.0f);

	// assign textures
	color = texture(texture_diffuse, fs_in.TexCoord).rgb;
	vec3 offsetPos = vec3(fs_in.Position.x, fs_in.Position.y + 0.25f, fs_in.Position.z);

    // Ambient
    vec3 ambient = 0.05 * color;
    // Diffuse
    //vec3 lightDir = normalize(lightPos - fs_in.Position);
    vec3 lightDir = normalize(lightPos - offsetPos);
	vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * specColour.rgb;
    // Specular
    //vec3 viewDir = normalize(viewPos - fs_in.Position);
    vec3 viewDir = normalize(viewPos - offsetPos);
	vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    vec3 specular = (specColour.rgb + vec3(-0.1, -0.3, 0.0)) * spec; // assuming bright white light color

	fragColor = vec4(ambient + diffuse + specular, 1.0f);
}
