#version 330 core

in VS_OUT {
	vec3 Position;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;
out vec4 color;

uniform sampler2D screenTexture;

uniform float standard;
uniform float invert;
uniform float greyScale;
uniform float sharpen;
uniform float blur;
uniform float edgeDetection;

// kernels
float sharpenKernel[9] = float[](
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);
float blurKernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);
float edgeKernel[9] = float[](
    1, 1, 1,
    1, -8, 1,
    1, 1, 1
);

// functions
const float offset = 1.0 / 300;
vec4 kernelEffect(float kernel[9]);
vec4 greyScaleImage(vec4 colour);

void main()
{
	vec4 inverted = vec4(0);
	vec4 greyed = vec4(0);
	vec4 sharpened = vec4(0);
	vec4 blured = vec4(0);
	vec4 edged = vec4(0);
	vec4 plain = vec4(0);


	if (invert == 1.0f) {
		inverted = vec4(vec3(1.0 - texture(screenTexture, fs_in.TexCoord)), 1.0);
	}
	if (greyScale == 1.0f && standard == 1.0f) {
	    greyed = greyScaleImage(texture(screenTexture, fs_in.TexCoord));
	}
	if (sharpen == 1.0f) {
		sharpened = kernelEffect(sharpenKernel);
	}
	if (blur == 1.0f) {
		blured = kernelEffect(blurKernel);
	}
	if (edgeDetection == 1.0f) {
		edged = kernelEffect(edgeKernel);
		if (greyScale == 1.0f) {
			edged = greyScaleImage(edged);
		}
	}
	if (standard == 1.0f) {
		color = texture(screenTexture, fs_in.TexCoord);
	}

	color = plain + edged + blured + sharpened + greyed + inverted;
}

vec4 kernelEffect(float kernel[9]) {
	    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(0.0f,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f,    0.0f),    // center-center
        vec2(offset,  0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right    
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, fs_in.TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    return vec4(col, 1.0);
}

vec4 greyScaleImage(vec4 colour) {
	//colour = texture(screenTexture, TexCoords);
	float average = 0.2126 * colour.r + 0.7152 * colour.g + 0.0722 * colour.b;
	colour = vec4(average, average, average, 1.0);
	return colour;
}
