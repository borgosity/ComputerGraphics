// shadertype = glsl
#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec2 vTexCoords;	// texture coordinates
out vec3 vSurfaceNorm;	// surface normals
out vec3 vToCamera;		// vector to the camera position
out vec3 vToLight;		// vector to the light source

// camera
uniform mat4 model;		// model transformation matrix
uniform mat4 view;		// camera view matrix
uniform mat4 projection;// camera projectionmatrix
uniform vec3 lightPos;	// scene light position

void main()
{
	// set gl position and pass texture coordinates
    vec4 worldPos = model * vec4(position, 1.0f);
	gl_Position = projection * view * worldPos;
	vTexCoords = texCoord;
	// calculate vectors
    vSurfaceNorm = (model * vec4(color, 0.0)).xyz;								// model surface normal vector
	vToLight  = lightPos - worldPos.xyz;										// vector pointing to light cource
	vToCamera = (inverse(view) * vec4(0.0f,0.0f,0.0f,1.0f)).xyz - worldPos.xyz; // vector to camera position (retrieve camera position from view matrix)
}
