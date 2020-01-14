#include "LoadObjModel.h"

LoadObjModel::LoadObjModel() : vertices(std::vector<glm::vec3>()), normals(std::vector<glm::vec3>()),
textureCoords(std::vector<glm::vec2>()), indices(std::vector<int>()), normalIndices(std::vector<int>()),
textureIndices(std::vector<int>()), meshVertices(std::vector<Vertex>()), subMeshes(std::vector<SubMesh>()), currentMaterial(Material()) {

}

LoadObjModel::~LoadObjModel()
{
	OnDestroy();
}

void LoadObjModel::LoadModel(const std::string& fileName_)
{
	std::ifstream in(fileName_, std::ios::in);
	if (!in) {
		Debug::Error("Cannot open OBJ file: " + fileName_, "LoadObjModel.cpp", __LINE__);
		return;
	}
	std::string line;
	while (std::getline(in, line)) {
		if (line.substr(0, 2) == "v ") {
			glm::vec3 vert;
			double x, y, z;
			std::istringstream v(line.substr(2));
			v >> x >> y >> z;
			vert = glm::vec3(x, y, z);
			if (boundingbox.minVert.x > vert.x) {
				boundingbox.minVert.x = vert.x;
			}
			if (boundingbox.minVert.y > vert.y) {
				boundingbox.minVert.y = vert.y;
			}
			if (boundingbox.minVert.z > vert.z) {
				boundingbox.minVert.z = vert.z;
			}
			if (boundingbox.maxVert.x < vert.x) {
				boundingbox.maxVert.x = vert.x;
			}
			if (boundingbox.maxVert.y < vert.y) {
				boundingbox.maxVert.y = vert.y;
			}
			if (boundingbox.maxVert.z < vert.z) {
				boundingbox.maxVert.z = vert.z;
			}
			vertices.push_back(vert);
		}
		// NORMAL DATA
		if (line.substr(0, 3) == "vn ") {
			glm::vec3 norm;
			double x, y, z;
			std::istringstream vn(line.substr(3));
			vn >> x >> y >> z;
			norm = glm::vec3(x, y, z);
			normals.push_back(norm);
		}
		// TEXTURE DATA
		if (line.substr(0, 3) == "vt ") {
			glm::vec2 texCoord;
			double x, y;
			std::istringstream vt(line.substr(3));
			vt >> x >> y;
			texCoord = glm::vec2(x, y);
			textureCoords.push_back(texCoord);
		}
		// FACE DATA
		// By vertex index/texture index/normal index
		if (line.substr(0, 2) == "f ") {
			char space;
			int vertIndices[3], texIndices[3], normIndices[3];
			std::istringstream f(line.substr(2));
			f >> vertIndices[0] >> space >> texIndices[0] >> space >> normIndices[0] >>
				vertIndices[1] >> space >> texIndices[1] >> space >> normIndices[1] >>
				vertIndices[2] >> space >> texIndices[2] >> space >> normIndices[2];

			indices.push_back(vertIndices[0] - 1);
			indices.push_back(vertIndices[1] - 1);
			indices.push_back(vertIndices[2] - 1);
			textureIndices.push_back(texIndices[0] - 1);
			textureIndices.push_back(texIndices[1] - 1);
			textureIndices.push_back(texIndices[2] - 1);
			normalIndices.push_back(normIndices[0] - 1);
			normalIndices.push_back(normIndices[1] - 1);
			normalIndices.push_back(normIndices[2] - 1);
		}
		// NEW MATERIAL (new mesh)
		if (line.substr(0, 7) == "usemtl ") {
			if (indices.size() > 0) {
				PostProcessing();
			}
			LoadMaterial(line.substr(7));
		}
	}
	PostProcessing();
}

void LoadObjModel::LoadModel(const std::string& fileName_, const std::string& matName_)
{
	LoadMaterialLibrary(matName_);
	LoadModel(fileName_);
}

std::vector<Vertex> LoadObjModel::GetVerts()
{
	return meshVertices;
}

std::vector<int> LoadObjModel::GetIndices()
{
	return indices;
}

std::vector<SubMesh> LoadObjModel::GetSubMeshes()
{
	return subMeshes;
}

void LoadObjModel::OnDestroy()
{
	vertices.clear();
	normals.clear();
	textureCoords.clear();
	indices.clear();
	normalIndices.clear();
	textureIndices.clear();
	meshVertices.clear();
	subMeshes.clear();
}

void LoadObjModel::PostProcessing()
{
	for (int i = 0; i < indices.size(); i++) {
		Vertex vert;
		vert.position = vertices[indices[i]];
		vert.normal = normals[normalIndices[i]];
		vert.texCoords = textureCoords[textureIndices[i]];
		meshVertices.push_back(vert);
	}
	SubMesh mesh;
	mesh.vertexList = meshVertices;
	mesh.meshIndices = indices;
	mesh.material = currentMaterial;
	subMeshes.push_back(mesh);

	indices.clear();
	normalIndices.clear();
	textureIndices.clear();

	currentMaterial = Material();
}

void LoadObjModel::LoadMaterial(const std::string& fileName_)
{
	currentMaterial = MaterialHandler::GetInstance()->GetMaterial(fileName_);
}

void LoadObjModel::LoadMaterialLibrary(const std::string& matName_)
{
	MaterialLoader::LoadMaterial(matName_);
}

BoundingBox LoadObjModel::GetBoundingBox()
{
	return boundingbox;
}

