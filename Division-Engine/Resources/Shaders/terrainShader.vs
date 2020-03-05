#version 450 core

// RENDERING PROCESS OF TERRAIN
// The code renders the terrain as a grid of patches, each of which can generate a grid of up to 64 x 64 triangles. 
// The patches are rendered using instancing, using the glDrawArraysInstanced() function with a dummy vertex buffer. 
// This means the whole terrain is rendered with a single draw call. 
// The actual patch origin positions are then calculated from the gl_InstanceID in the vertex shader.

layout(location = 31) uniform ivec2 pixelGridSize;		// PATCHES SIZE
layout(location = 33) uniform vec2 terrainSizeWorld;	// WORLD SIZE

// SETTING VEC3 ARRAYS OF QUAD PATCHES
const vec3 quadPatches[] = vec3[](
	vec3(-0.5,  0.0,  0.5),
	vec3( 0.5,  0.0,  0.5),
	vec3( 0.5,  0.0, -0.5),
	vec3(-0.5,  0.0, -0.5));

const vec2 HALF_COORDS = vec2(0.5, 0.5);
vec2 terrainOffset = vec2(0.0, 0.0);

out TerrainVertOut {
	vec2 texCoordNorm;
	vec2 texCoordAlbedo;
} tvs_out;


void main() {
	// DIVIDE WORLD TERRAIN SIZE BY EACH PIXEL GRID SIZE 
	const vec2 patchSizeWorld = terrainSizeWorld / vec2(pixelGridSize);

	// TERRAIN OFFSETS
	terrainOffset.x = float(gl_InstanceID& (pixelGridSize.x -1)) * patchSizeWorld.x; // gl_InstanceID — contains the index of the current primitive in an instanced draw command
	terrainOffset.y = floor(float(gl_InstanceID) / gridSize.x) * patchSizeWorld.y;

	// TERRAIN DISPLACEMENT WITH NORMAL TEXTURE
	tvs_out.texCoordNorm = (((quadPatches[gl_VertexID].xz + HALF_COORDS) * patchSizeWorld) + terrainOffset) / terrainSizeWorld;
	tvs_out.texCoordNorm.t = 1.0 - tvs_out.texCoord.t; // TEXTURE COORDINATE OF t

	// TERRAIN ALBEDO TEXTURE
	tvs_out.texCoordAlbedo = quadPatches[gl_VertexID].xz + HALF_COORDS;
	tvs_out.texCoordAlbedo.t = 1.0 - vs_out.texCoordAlbedo.t;

	// TERRAIN INSTANCING FOR POSITION
	// SETTING Y AXIS WITH STATIC VALUE
	vec3 instPos = (quadPatches[gl_VertexID] * vec3(patchSizeWorld.x, 1, patchSizeWorld.y)) + vec3(terrainOffset.x, 0, terrainOffset.y);

	// TERRAIN INSTANCING POS TO STAY AT CENTER BY (0, 0);
	instPos.xz -= (terrainSizeWorld / 2.0) - (patchSizeWorld / 2.0);

	// FINAL RESULT OF gl_Position of vec4 w
	gl_Position = vec4(instPos, 1.0);
}