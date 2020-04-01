#version 450 core

// A Geometry Shader (GS) is a Shader program written in GLSL 
// that governs the processing of Primitives. Geometry shaders reside between the 
// Vertex Shaders (or the optional Tessellation stage) 
// and the fixed-function Vertex Post-Processing stage.
// Each geometry shader is designed to accept a specific Primitive type as input and to output a specific primitive type. 
// The accepted input primitive type is defined in the shader:
// layout(input_primitive) in;
// GS inputs: points, lines, lines_adjacency, triangles, triangles_adjacency

layout (triangles) in;

// The vertex count is the number of vertices that the GS 
// receives per-input primitive.
// The output primitive type is defined as follows:
// layout(output_primitive?, max_vertices = vert_count?) out;

layout(triangle_strip, max_vertices = 3) out;

// UNIFORM LOCATIONS FOR TERRAIN GEOMETRY SHADER
layout(location = 10) uniform mat4 shadowMat; // Remapping [-1, 1] to [0, 1]
layout(location = 11) uniform mat4 viewMat;
layout(location = 13) uniform mat4 projMat;
layout(location = 30) uniform ivec2 viewportSize;
layout(location = 40) uniform vec4 plane;

// RECEIVE TES OUTs
in TerrainTESOut {
	vec2 texCoordNorm;
	vec2 texCoordAlbedo;
} gs_in[];

// SEND OUT GS TO FS
out TerrainGSOut {
	vec3 eyePos;
	vec3 worldPos;
	vec4 shadowMappingPos;
	vec2 texCoordNorm;
	vec2 texCoordAlbedo;
	noperspective vec3 gEdgeDistance; // The value will be linearly interpolated in window-space. This is usually not what you want, but it can have its uses.
} gs_out;

// MULTIPLY PROJ AND VIEW MATRIX
mat4 projViewMat = projMat * viewMat;

vec2 ViewportProjection(vec4 p) {
	vec4 clipSpace = projViewMat * p;
	clipSpace.xy /= clipSpace.w;
	clipSpace.xy = ((clipSpace.xy * 0.5) + 0.5) * viewportSize;	
	return clipSpace.xy;
}

void VertEmission(int v, vec3 gEdgeDistances) {
	gs_out.worldPos = gl_in[v].gl_Position.xyz;
	gs_out.eyePos = (viewMat * gl_in[v].gl_Position).xyz;
	gs_out.shadowMappingPos = shadowMat * gl_in[v].gl_Position;

	gs_out.texCoordNorm = gs_in[v].texCoordNorm;
	gs_out.texCoordAlbedo = gs_in[v].texCoordAlbedo;

	gs_out.gEdgeDistance = gEdgeDistances;

	gl_Position = projViewMat * gl_in[v].gl_Position;

	gl_ClipDistance[0] = dot(gl_in[v].gl_Position, plane);

	// EmitVertex — emit a vertex to the first vertex stream
	// emits the current values of output variables to the current output primitive on 
	// the first (and possibly only) primitive stream. 
	// It is equivalent to calling EmitStreamVertex with stream set to 0.
	// Source - https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/EmitVertex.xhtml
	EmitVertex();
}

void main() {
	
	// DEFINING PRIMS
	vec2 prim0 = ViewportProjection(gl_in[0].gl_Position);
	vec2 prim1 = ViewportProjection(gl_in[1].gl_Position);
	vec2 prim2 = ViewportProjection(gl_in[2].gl_Position);

	// DEFINING EDGES
	vec2 edge0 = prim1 - prim0;
	vec2 edge1 = prim2 - prim1;
	vec2 edge2 = prim2 - prim0;

	// DEFINING DOUBLE TRIANGLE AREA
	// 1/2 * abs(cross(u, v)) with u and v being vectors from the same vertex
	float dTriangle = abs((edge0.x * edge2.y) - (edge0.y * edge2.x));

	// DEFINING TRIANGLE AREA
	// Triangle area = Base * Height / 2,  Height = 2 * Area / Base
	float height0 = dTriangle / length(edge0);
	float height1 = dTriangle / length(edge1);
	float height2 = dTriangle / length(edge2);

	// EMIT VERTEX AND END PRIMITIVE
	VertEmission(0, vec3(0, height1, 0));
	VertEmission(1, vec3(0, 0, height2));
	VertEmission(2, vec3(height0, 0, 0));

	// EndPrimitive — complete the current output primitive on the first vertex stream
	// completes the current output primitive on the first (and possibly only) vertex stream and starts a new one. 
	// No vertex is emitted. 
	// Calling EndPrimitive is equivalent to calling EmitStreamVertex with stream set to 0.
	// Source - https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/EndPrimitive.xhtml
	EndPrimitive();
}

