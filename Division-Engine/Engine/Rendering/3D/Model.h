#ifndef MODEL_H
#define MODEL_H

#include "../../../DivisionPCH.h"
#include "LoadObjModel.h"

class Model
{
public:
	Model(const std::string& objpath_, const std::string& matPath_, GLuint shaderProgram_);
	~Model();
	void AddMesh(Mesh* mesh_);
	void Render(Camera* camera_);
	void OnDestroy();
	int CreateInstance(glm::vec3 position_, float angle_, glm::vec3 rotation_, glm::vec3 scale_); // Grab all data from ObjLoader 
	void UpdateInstance(int index_, glm::vec3 position_, float angle_, glm::vec3 rotation_, glm::vec3 scale_);

	inline glm::mat4 GetTransform(int index_) const { return modelInstances[index_]; }
	inline BoundingBox GetBoundingBox() { return boundingBox; };
	inline GLuint GetShaderProgram() const { return shaderProgram; };
private:
	std::vector<Mesh*> subMeshes;
	GLuint shaderProgram;
	std::vector<glm::mat4> modelInstances;

	glm::mat4 GetTransform(glm::vec3 position_, float angle_, glm::vec3 rotation_, glm::vec3 scale_) const;
	void LoadModel();
	LoadObjModel* obj;
	BoundingBox boundingBox;
};
#endif // !1


