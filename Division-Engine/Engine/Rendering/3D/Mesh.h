#ifndef MESH_H
#define MESH_H

#include "../../../DivisionPCH.h"

#include "../../Core/Entity/Entity.h"
#include "../Buffers/IndexBufferObject.h"
#include "../Buffers/VertexBufferObject.h"

// USING ASSIMP OPEN SOURCE TO RENDER MESH
class Mesh : public Entity {
public:
	Mesh();
	virtual ~Mesh();

	void OnDestroy();

	// INIT SINGLE FILE LOCATION (SUCH AS OBJ)
	void InitFromFile(const std::string& path_);
	// INIT RAW FILE DATAS (VERTEX NUMBERS AND INDICES NUMBER) 
	void InitFromRawData(const int verticesNum_, const int indicesNum_);
	// All the attribute data VBO VERTEX DATA
	void SetAttribute(int attributeID_, const void* data_);
	// Atribute data for single vertex
	void VertexAttribute(int attributeID_, int vertexIndex_, const void* data_);
	// Add Attribute data by elements
	int AddAttribute(unsigned int elementNum_, unsigned int elementByteSize_, 
		GLenum type_ = GL_FLOAT, GLboolean normalized_ = GL_FALSE);
	// RELEASE MEMORY FOR RAW DATA INIT
	void EndFromRawData();
	// ARRAY OF INDICES ADDITION
	void SetIndices(GLuint* indices_);
	// SINGLE INDEX  ADDITION
	void AddIndex(GLuint index_);
	// RENDER BY GL_TRIANGLES
	void RenderIndex(GLenum mode_ = GL_TRIANGLES) const;
	void Render(GLenum mode_ = GL_TRIANGLES) const;

	// WHEN RENDERING MULTIPLE INSTANCE PROPERTY
	void RenderInstance(GLenum mode_ = GL_TRIANGLES) const;
	void SetRenderInstanceProp(int instanceCount_, int index_, int divisor_);

	// For Normal Mapping
	inline void SetComputeTangents(bool t_) { computeTangents = t_; }

private:
	void InitAssimpScene(const aiScene* scene_);

	// Purpose for SubMeshes
	std::vector<IndexBufferObject*> iboVector;
	std::vector<VertexBufferObject*> vboVector;

	// For Rendering Instance purposes
	int instanceCount;
	bool computeTangents;
};


#endif // !MESH_H
