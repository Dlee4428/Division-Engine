#version 450 core

// UNIFORM BINDING FOR TEXTURE NORMAL AND ALBEDO
layout(binding = 1) uniform sampler2D textureNorm;
layout(binding = 2) uniform sampler2DArray textureAlbedo;

// UNIFORM BINDING FOR SHADOWMAPPING
// If a texture has a depth or depth-stencil image format and has the depth comparison activated, it cannot be used with a normal sampler. 
// Attempting to do so results in undefined behavior. Such textures must be used with a shadow sampler. 
// This type changes the texture lookup functions (see below), adding an additional component to the textures' usual texture coordinate vector. 
// This extra value is used to compare against the value sampled from the texture.
// Source - https://www.khronos.org/opengl/wiki/Sampler_(GLSL)
layout(binding = 3) uniform sampler2DShadow shadowMapping;

// UNIFORM LOCATION FOR TERRAIN FRAGMENT SHADER
layout(location = 13) uniform mat4 shadowMat;
layout(location = 20) uniform vec3 directionalLight; // Sun Direction
layout(location = 21) uniform vec3 directionalLightColor; // Sun Light Color
layout(location = 32) uniform float scaleDisplacement;
layout(location = 35) uniform bool isFogEnable;
layout(location = 36) uniform bool wireFrameMode;
layout(location = 37) uniform bool isShadowMapping;

// RECEIVE OUTS FROM GS
in TerrainGSOut {
	vec2 texCoordNorm;
	vec2 texCoordAlbedo;
	vec3 eyePos;
	vec3 worldPos;
	vec4 shadowMappingPos;
	noperspective vec3 gEdgeDistance; // The value will be linearly interpolated in window-space. This is usually not what you want, but it can have its uses.
} fs_in;

out vec4 fragColor;
float shadowFactor = 1.0;

// PARAM - Y VERT POS, THRESHHOLD FOR MULTITEXTURE HEIGHT BASED RENDERING
vec4 BlendTextureAlbedo(float vert_, float thres1_, float thres2_, float thres3_, float transition_) {
	// BLENDING TEXTURE IF ELSE STATEMENT TO CAP WHERE TO BLEND EACH STAGE OF HEIGHT
	// THRESHOLD STAGE 1
	if (vert_ < thres1_ - transition_) {
		return texture(textureAlbedo, vec3(fs_in.texCoordAlbedo, 0.0));
	}
	else if (vert_ < thres1_ + transition_) {
		float factor1 = (vert_ - (thres1_ - transition_)) / (2.0 * transition_);
		return mix(
			texture(textureAlbedo, vec3(fs_in.texCoordAlbedo, 0.0)), 
			texture(textureAlbedo, vec3(fs_in.texCoordAlbedo, 1.0)), 
			factor1);
	}
	// THRESHOLD STAGE 2
	else if (vert_ < thres2_ - transition_) {
		return texture(textureAlbedo, vec3(fs_in.texCoordAlbedo, 1.0));
	}
	else if (vert_ < thres2_ + transition_) {
		float factor2 = (vert_ - (thres2_ - transition_)) / (2.0 * transition_);
		return mix(
			texture(textureAlbedo, vec3(fs_in.texCoordAlbedo, 1.0)), 
			texture(textureAlbedo, vec3(fs_in.texCoordAlbedo, 2.0)), 
			factor2);
	}
	// THRESHOLD STAGE 3
	else if (vert_ < thres3_ - transition_) {
		return texture(textureAlbedo, vec3(fs_in.texCoordAlbedo, 2.0));
	}
	else if (vert_ < thres3_ + transition_) {
		float factor3 = (vert_ - (thres3_ - transition_)) / (2.0 * transition_);
		return mix(
			texture(textureAlbedo, vec3(fs_in.texCoordAlbedo, 2.0)), 
			texture(textureAlbedo, vec3(fs_in.texCoordAlbedo, 3.0)), 
			factor3);
	}
	else {
		return texture(textureAlbedo, vec3(fs_in.texCoordAlbedo, 3.0));
	}
}

void main() {
	
	if (isShadowMapping) {
		// TERRAIN SHADOW MAPPING
		// textureProjOffset divides each coordinate component by their last value, 
		// and it is performing perspective division
		// the resulting 3rd component is used in the depth comparison 
		// (because its a shadow sampler with depth comparison)
		// 4 texels sampled diagonally. If filtering is enabled this yields 16 texels
		float result = 0;
		result += textureProjOffset(shadowMapping, fs_in.shadowMappingPos, ivec2(-1, -1));
		result += textureProjOffset(shadowMapping, fs_in.shadowMappingPos, ivec2( 1, -1));
		result += textureProjOffset(shadowMapping, fs_in.shadowMappingPos, ivec2(-1,  1));
		result += textureProjOffset(shadowMapping, fs_in.shadowMappingPos, ivec2( 1,  1));
		shadowFactor = result * 0.25;
	}
	//////////////////////////////////////////////////
	// TERRAIN LIGHTING
	//////////////////////////////////////////////////
	// NORMAL MAPPING
	// Source - https://learnopengl.com/Advanced-Lighting/Normal-Mapping
	// Obtain normal from normal map in range [0, 1]
	vec3 norm = texture(textureNorm, fs_in.texCoordNorm).rgb;
	// Transform normal vector to range [-1, 1];
	norm = normalize(norm * 2.0 - 1.0);
	//////////////////////////////////////////////////
	// GENERAL LIGHTING FROM THE SUN DIRECTION
	vec3 ambient = directionalLightColor * 0.15;
	vec3 diffuse = directionalLightColor * max(0.0, dot(norm, -directionalLight));
	vec3 light = ambient + shadowFactor * diffuse;
	//////////////////////////////////////////////////
	// HEIGHT BASED MULTILAYER TEXTURE
	float height = fs_in.worldPos.y;
	const float thres1 = scaleDisplacement * 0.15;
	const float thres2 = scaleDisplacement * 0.45;
	const float thres3 = scaleDisplacement * 0.9;
	const float transition = scaleDisplacement * 0.1;

	vec4 albedoTex = BlendTextureAlbedo(height, thres1, thres2, thres3, transition);
	float texSlope = 1.0 - norm.y;
	albedoTex = mix(albedoTex, texture(textureAlbedo, vec3(fs_in.texCoordAlbedo, 3.0)), texSlope * 0.15);
	
	fragColor = vec4(light, 1.0) * albedoTex;

	// WIREFRAME MODE
	if (wireFrameMode) {
		const vec4 wireframeColor = vec4(0.0, 0.0, 0.0, 0.0);
		const float wireframeLineSize = 1.0;
		float wireframeStrength = 0.0;

		float fDistToEdge = min(fs_in.gEdgeDistance.x, min(fs_in.gEdgeDistance.y, fs_in.gEdgeDistance.z));
		if (fDistToEdge <= wireframeLineSize) {
			wireframeStrength = smoothstep(0.0, 1.0, 1.0 - (fDistToEdge / wireframeLineSize));
		}
		fragColor = wireframeColor * wireframeStrength + (1.0 - wireframeStrength) * fragColor;
	}

	// FOG MODE
	if (isFogEnable) {
		// Compute range based distance
		float dist = length(fs_in.eyePos);

		// Fog factors
		// smoothstep — perform Hermite interpolation between two values
		// smoothstep performs smooth Hermite interpolation between 0 and 1 when edge0 < x < edge1. 
		// This is useful in cases where a threshold function with a smooth transition is desired.
		// Source - https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/smoothstep.xhtml
		float be = 0.0005 * smoothstep(0.0, scaleDisplacement, scaleDisplacement - fs_in.worldPos.y);
		float bi = 0.0002 * smoothstep(0.0, scaleDisplacement, scaleDisplacement - fs_in.worldPos.y);
		
		float ext = exp(-dist * be); // extintion
		float insc = exp(-dist * bi); // inscattering
		const vec4 fogColor = vec4(0.7, 0.8, 0.9, 1.0);
		fragColor = fragColor * ext + fogColor * (1.0 - insc);
	}
}

