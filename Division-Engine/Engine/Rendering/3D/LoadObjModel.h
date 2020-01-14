#ifndef LOADOBJMODEL_H
#define LOADOBJMODEL_H

#include "../../../DivisionPCH.h"
#include "Mesh.h"
#include "../../FX/MaterialLoader.h"
#include "../../Math/BoundingBox.h"

class LoadObjModel
{
public:
	LoadObjModel();
	~LoadObjModel();
	void LoadModel(const std::string& fileName_);
	void LoadModel(const std::string& fileName_, const std::string& matName_);
	std::vector<Vertex> GetVerts();
	std::vector<int> GetIndices();
	std::vector<SubMesh> GetSubMeshes();
	BoundingBox GetBoundingBox();
	std::stack<BoundingBox> boundingStack;
	void OnDestroy();
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::vector<int> indices;
	std::vector<int> normalIndices;
	std::vector<int> textureIndices;
	std::vector<Vertex> meshVertices;
	std::vector<SubMesh> subMeshes;

	void PostProcessing();
	void LoadMaterial(const std::string& fileName_);
	void LoadMaterialLibrary(const std::string& matName_);

	Material currentMaterial;
	BoundingBox boundingbox;
};

#endif // !LOADOBJMODEL_H
