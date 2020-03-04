#version 450 core
///////////////////////////////////////////////////////////
// A Geometry Shader (GS) is a Shader program written in GLSL 
// that governs the processing of Primitives. Geometry shaders reside between the 
// Vertex Shaders (or the optional Tessellation stage) 
// and the fixed-function Vertex Post-Processing stage.
///////////////////////////////////////////////////////////
// Each geometry shader is designed to accept a specific Primitive type as input and to output a specific primitive type. 
// The accepted input primitive type is defined in the shader:
// layout(input_primitive) in;
// GS inputs: points, lines, lines_adjacency, triangles, triangles_adjacency
///////////////////////////////////////////////////////////
layout(triangles) in;

///////////////////////////////////////////////////////////
// The vertex count is the number of vertices that the GS 
// receives per-input primitive.
// The output primitive type is defined as follows:
// layout(output_primitive​, max_vertices = vert_count​) out;
///////////////////////////////////////////////////////////
layout(triangle_strip, max_vertices = 3) out;

// UNIFORM LOCATIONS FOR TERRAIN GEOMETRY SHADER
layout(location = 11) uniform mat4 projMat;
layout(location = 12) uniform mat4 viewMat;
layout(location = 13) uniform mat4 shadowMat; // Remapping [-1, 1] to [0, 1]
layout(location = 15) uniform ivec2 viewportSize;

// RECEIVE TES OUTs
out TerrainTESOut {
	vec2 texCoordNorm;
	vec2 texCoordAlbedo;
} gs_in[];

// SEND OUT GS TO FS
out TerrainGSOut {
	vec2 texCoordNorm;
	vec2 texCoordAlbedo;
	vec3 eyePos;
	vec3 worldPos;
	vec4 shadowMappingPos;
	noperspective vec3 gEdgeDistance; // The value will be linearly interpolated in window-space. This is usually not what you want, but it can have its uses.
} gs_out;

// MULTIPLY PROJ AND VIEW MATRIX
mat4 projViewMat = projMat * viewMat;

void vertEmission(int vert_, vec3 gEdgeDistance_) {
	gs_out.texCoordNorm = gs_in[vert_].texCoordNorm;
	gs_out.texCoordAlbedo = gs_in[vert_].texCoordAlbedo;

	gs_out.eyePos = (viewMat * gl_in[vert_].gl_Position).xyz;
	gs_out.worldPos = gl_in[vert_].gl_Position.xyz;
	gs_out.shadowMappingPos = shadowMat * gl_in[vert_].gl_Position;

	gs_out.gEdgeDistance = gEdgeDistance_;

	gl_Position = projViewMat * gl_in[vert_].gl_Position;

	// EmitVertex — emit a vertex to the first vertex stream
	// emits the current values of output variables to the current output primitive on 
	// the first (and possibly only) primitive stream. 
	// It is equivalent to calling EmitStreamVertex with stream set to 0.
	// Source - https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/EmitVertex.xhtml
	EmitVertex();
}

vec2 viewportProjection(vec4 p_) {
	vec4 clipSpace = projViewMat * p_;
	clipSpace.xy /= clipSpace.w;
	clipSpace.xy = ((clipSpace.xy * 0.5) + 0.5) * viewportSize;
	
	return clipSpace.xy;
}

void main() {
	
	// DEFINING PRIMS
	vec2 prim1 = viewportProjection(gl_in[0].gl_Position);
	vec2 prim2 = viewportProjection(gl_in[1].gl_Position);
	vec2 prim3 = viewportProjection(gl_in[2].gl_Position);

	// DEFINING EDGES
	vec2 edge1 = prim2 - prim1;
	vec2 edge2 = prim3 - prim2;
	vec2 edge3 = prim3 - prim1;

	// DEFINING DOUBLE TRIANGLE AREA
	// 1/2 * abs(cross(u, v)) with u and v being vectors from the same vertex

}

