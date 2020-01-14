#ifndef MATERIALHANDLER_H
#define MATERIALHANDLER_H

#include "../../DivisionPCH.h"

struct Material {
	GLuint diffuseMap;

	float shininess; //Ns
	float transparency; //d

	glm::vec3 ambient; //Ka
	glm::vec3 diffuse; //Kd
	glm::vec3 specular; //Ks

	inline Material() : diffuseMap(0), shininess(0.0f), transparency(0.0f),
		ambient(glm::vec3(1.0f)), diffuse(glm::vec3(1.0f)), specular(glm::vec3()) {}
};

class MaterialHandler
{
public:
	//Singleton design pattern
	MaterialHandler(const MaterialHandler&) = delete;
	MaterialHandler(MaterialHandler&&) = delete;
	MaterialHandler& operator=(const MaterialHandler&) = delete;
	MaterialHandler& operator=(MaterialHandler&&) = delete;

	static MaterialHandler* GetInstance();

	void AddMaterial(const std::string& name_, Material mat_);
	const Material GetMaterial(const std::string& name_);
	void OnDestroy();
private:
	MaterialHandler();
	~MaterialHandler();

	static std::unique_ptr<MaterialHandler> materialInstance;
	friend std::default_delete<MaterialHandler>;

	static std::map<std::string, Material> materials;
};
#endif 