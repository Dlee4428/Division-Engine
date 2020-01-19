#version 450 core

// Ratio setters
const float etaR = 1.14;
const float etaG = 1.32;
const float etaB = 1.10;

// Calculate Chromatic result
const float fresnelPower = 2.0;
const float result = ((1.0 - etaG) * (1.0 - etaG)) / ((1.0 + etaG) * (1.0 + etaG));

out vec4 fragColor;

in vec3 vNormal;
in vec3 vPosition;

uniform vec3 viewPos;
uniform samplerCube refTexture;
uniform bool chromatic;

void main()
{
	// Direction vector
	vec3 dirVector = normalize(viewPos - vPosition);

	// Ratio variables
	float ratio = 1.32 / 1.52; 
	float chromeRatio = result + (1.0 - result) * pow(1.0 - dot(dirVector, vNormal), fresnelPower);

	// Set Reflection && Refraction Vector
	vec3 reflectionVector = reflect(dirVector, normalize(vNormal));
	vec3 refractionVector = refract(dirVector, normalize(vNormal), 1.0);
	
	// Convert to Vec4
	vec4 reflectionColor = texture(refTexture, reflectionVector);
	vec4 refractionColor = texture(refTexture, refractionVector);
	
	if(!chromatic)
	{
		// Refraction & Reflection S	
		vec3 combinedColor = mix(vec3(refractionColor), vec3(reflectionColor), ratio);
		fragColor = vec4(combinedColor, 1.0);
	} 
	else 
	{
		// Chromatic Aberration
		vec3 combinedColor = mix(vec3(refractionColor), vec3(reflectionColor), chromeRatio);
		fragColor = vec4(combinedColor, 1.0);
	}
}