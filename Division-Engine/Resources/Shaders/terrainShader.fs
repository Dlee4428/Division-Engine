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
layout(location = 20) uniform vec3 DirectionalLight; // Sun Direction
layout(location = 21) uniform vec3 DirectionalLightColor; // Sun Light Color
layout(location = 32) uniform float scaleDisplacement;
layout(location = 35) uniform bool isFogEnable;
layout(location = 36) uniform bool wireFrameMode;
layout(location = 37) uniform bool isShadowMapping;

// RECEIVE OUTS FROM GS
out TerrainGSOut {
	vec2 texCoordNorm;
	vec2 texCoordAlbedo;
	vec3 eyePos;
	vec3 worldPos;
	vec4 shadowMappingPos;
	noperspective vec3 gEdgeDistance; // The value will be linearly interpolated in window-space. This is usually not what you want, but it can have its uses.
} fs_in;

out vec4 fragColor;

// PARAM - Y VERT POS, THRESHHOLD FOR MULTITEXTURE HEIGHT BASED RENDERING
vec4 BlendTextureAlbedo(float vert_, thres1_, thres2_, thres3_, float transition_) {
	// BLENDING TEXTURE IF ELSE STATEMENT TO CAP WHERE TO BLEND EACH STAGE OF HEIGHT
	// THRESHOLD STAGE 1
	if (vert_ < thres1_ - transition_) {
		return texture(textureAlbedo, vec3(fs_in.texCoordAlbedo, 0.0);
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

}

