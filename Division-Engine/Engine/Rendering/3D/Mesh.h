#ifndef MESH_H
#define MESH_H

#include "../../../DivisionPCH.h"
#include "../../Core/Entity/Entity.h"
#include "../Buffers/IBO.h"
#include "../Buffers/VBO.h"


// Implementing Assimp for Mesh
class Mesh : public Entity {
public:
	Mesh();
	virtual ~Mesh();

	void OnDestroy();

	void InitFromFile(const std::string& path_);
	void InitFromRawData(const int verticesNum_, const int indicesNum_);

	// All the attribute data
	void SetAttribute(int attributeID_, const void* data_);
	// Atribute data for single vertex
	void VertexAttribute(int attributeID_, int vertexIndex_, const void* data_);
	int AddAttribute(unsigned int elementNum_, unsigned int elementByteSize_, 
		GLenum type_ = GL_FLOAT, GLboolean normalized_ = GL_FALSE);

	void EndFromRawData();

	void SetIndices(GLuint* indices_);
	void AddIndex(GLuint index_);

	void RenderIndex(GLenum mode_ = GL_TRIANGLES) const;
	void Render(GLenum mode_ = GL_TRIANGLES) const;
	void RenderInstance(GLenum mode_ = GL_TRIANGLES) const;

	void SetRenderInstanceProp(int instanceCount_, int index_, int divisor_);

	// For Normal Mapping
	inline void SetComputeTangents(bool t_) { computeTangents = t_; }

private:
	void InitAssimpScene(const aiScene* scene_);

	// Purpose for SubMeshes
	std::vector<IBO*> iboVector;
	std::vector<VBO*> vboVector;

	// For Rendering Instance purposes
	int instanceCount;
	bool computeTangents;
};


#endif // !MESH_H
