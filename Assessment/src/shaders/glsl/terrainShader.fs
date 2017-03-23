// shadertype = glsl
#version 410 core

in VS_OUT {
	vec3 Position;
	vec3 Normal;
	vec2 TexCoord;
	float yHeight;
} fs_in;


out vec4 fragColor;

// light
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;

// terrain uniforms
uniform vec3 mapTex;


// Texture samplers
uniform sampler2D texture_grass;
uniform sampler2D texture_sand;
uniform sampler2D texture_dirt;
uniform sampler2D texture_water;
uniform sampler2D texture_snow;
uniform sampler2D texture_perlin;

vec4 setTextures(vec2 tileTex);
vec4 setTextures(vec2 tileTex, vec3 terrainMap);
vec3 getTerrainMap(float height);

void main()
{
    // set a default color to catch bugs
	vec3 color = vec3(0.1f, 0.2f, 0.3f);
	// set textures to tile by myuliplying the texture coordinates greater than 1
	vec2 tileTex = fs_in.TexCoord * 64.0f;
	// assign textures
	vec3 terrainMap = getTerrainMap(fs_in.yHeight);
	//vec4 texColor = setTextures(tileTex, terrainMap);
	vec4 texColor = setTextures(tileTex);
	
	//color = setTextures(tileTex);
	//color = texture(texture_perlin, fs_in.TexCoord).rgb;

    // Ambient
    vec3 ambient = 0.05 * color;
    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.Position);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.Position);
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
    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
    //fragColor = vec4(ambient + diffuse + specular, 1.0f);
	fragColor = vec4(texColor.rgb + diffuse + specular, texColor.a);
}

// function assigns textures depending of the heightMap
vec4 setTextures(vec2 tileTex) {
	vec4 color;
	if (fs_in.yHeight < mapTex.x)
		color = texture(texture_water, tileTex) * fs_in.yHeight;
	else if (fs_in.yHeight < (mapTex.x + 0.025f))
		color = texture(texture_sand, tileTex) * fs_in.yHeight;
	else if (fs_in.yHeight < mapTex.y )
		color = texture(texture_grass, tileTex) * fs_in.yHeight;
	else if (fs_in.yHeight < (mapTex.y + 0.025f))
		color = texture(texture_dirt, tileTex) * fs_in.yHeight;
	else
		color = texture(texture_snow, tileTex) * fs_in.yHeight;
	return color;
}

vec4 setTextures(vec2 tileTex, vec3 terrainMap){
	vec4 colorR = texture(texture_water, tileTex) * terrainMap.r;
	vec4 colorG = texture(texture_sand, tileTex) * terrainMap.g;
	vec4 colorB = texture(texture_grass, tileTex) * terrainMap.b;
	vec4 result = colorR + colorG + colorB;
	return result;
}

vec3 getTerrainMap(float height) {
	vec3 textureMap = vec3(mod(height, 0.3f),
							mod(height, 0.3f),
							mod(height, 0.3f));
	return textureMap;
}
