// shadertype = glsl
#version 330 core

// struct for materials
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
	sampler2D texture_emissive1;
    float shininess;
}; 
// directional light struct
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// point light struct
struct PointLight{
    vec3 position;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;

};
// uniforms for multiple point lights
uniform int pointLightCount;
uniform PointLight pointLights[pointLightCount];
// spot light struct
struct SpotLight{
	vec3 position;
    vec3 direction;

	float cutOff;
	float outerCutOff;
	
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;

};
// values passed from vector shader
in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
// final output value
out vec4 color;
// struct uniforms
uniform Material material;
uniform DirLight dirlight;
uniform PointLight pointlight;
uniform SpotLight spotlight;

uniform vec3 viewPos;
uniform int hasEmissive;

// function prototypes
vec3 calcutaleDirectionalLight(DirLight a_light, vec3 a_normal, vec3 a_viewDir);
vec3 calculatePointLight(PointLight a_light, vec3 a_normal, vec3 a_fragPos, vec3 a_viewDir);
vec3 calculateSpotLight(SpotLight a_light, vec3 a_normal, vec3 a_fragPos, vec3 a_viewDir);


void main()
{
	// function attributes
	vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
	// return value
	vec3 result;

	// Directional light
	result += calcutaleDirectionalLight(dirLight, normal, viewDir);
	// Point lights
	for (int i = 0; i < pointLightCount; i++) {
		result += calculatePointLight(pointLights[i], normal, FragPos, viewDir);
	}
	// Spot light
	result += calculateSpotLight(spotLight, normal, FragPos, viewDir);

	// add emmisive is there is any
	if (hasEmissive == 0){
		color = vec4(result, 1.0f); 
	}
	else {
		// Emission
		vec3 emission = vec3(texture(material.texture_emissive1, TexCoords)); 
		emission *= attenuation;
		// final colour
		color = vec4(result + emission, 1.0f);  
	}
}
//=========================== Directional Light function =========================== 
vec3 calcutaleDirectionalLight(DirLight a_light, vec3 a_normal, vec3 a_viewDir){
    // Ambient
    vec3 ambient = a_light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
  	
    // Diffuse 
    vec3 lightDir = normalize(-a_light.direction);  
    float diff = max(dot(a_normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));  
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, a_normal);  
    float spec = pow(max(dot(a_viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = a_light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

	return (ambient + diffuse + specular);
}
//=========================== Point Light function ================================= 
vec3 calculatePointLight(PointLight a_light, vec3 a_normal, vec3 a_fragPos, vec3 a_viewDir){
    
	// Ambient
    vec3 ambient = a_light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
  	
    // Diffuse 
    vec3 lightDir = normalize(-a_light.direction - a_fragPos);  
    float diff = max(dot(a_normal, lightDir), 0.0);
    vec3 diffuse = a_light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));  
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, a_normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = a_light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

	// Attenuation
    float distance    = length(-a_light.direction - a_fragPos);
    float attenuation = 1.0f / (a_light.constant + a_light.linear * distance + a_light.quadratic * (distance * distance));    

    ambient  *= attenuation;  
    diffuse  *= attenuation;
    specular *= attenuation;

	return (ambient + diffuse + specular);
}
//===========================  Spot Light function =================================
vec3 calculateSpotLight(SpotLight a_light, vec3 a_normal, vec3 a_fragPos, vec3 a_viewDir){
    
	vec3 lightDir = normalize(a_light.position - a_fragPos);

	// Check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-a_light.direction)); 

    if(theta > a_light.cutOff) //angles as cosines instead of degrees so a '>' is used.
    { 
		// Ambient
		vec3 ambient = a_light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
  	
		// Diffuse   
		float diff = max(dot(a_normal, lightDir), 0.0);
		vec3 diffuse = a_light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));  
    
		// Specular
		vec3 reflectDir = reflect(-lightDir, a_normal);  
		float spec = pow(max(dot(a_viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = a_light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

		return  (ambient + diffuse + specular);
	}
	else {
		// else, use ambient light so scene isn't completely dark outside the spot light.
        return (a_light.ambient * vec3(texture(material.diffuse, TexCoords)));
	}
}