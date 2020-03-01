#include "EntityManager.h"

class MaterialHandler;
class Mesh;
class ShaderProgram;
class Shader;
class Tex2D;
class Tex2DArray;
class TexCubemap;

// GLOBAL TEMPLATE FOR ENTITY MANAGER
// GETTER ENTITIES
template MaterialHandler* EntityManager::GetEntity<MaterialHandler>(const std::string& name_);
template Mesh* EntityManager::GetEntity<Mesh>(const std::string& name_);
template ShaderProgram* EntityManager::GetEntity<ShaderProgram>(const std::string& name_);
template Shader* EntityManager::GetEntity<Shader>(const std::string& name_);
template Tex2D* EntityManager::GetEntity<Tex2D>(const std::string& name_);
template Tex2DArray* EntityManager::GetEntity<Tex2DArray>(const std::string& name_);
template TexCubemap* EntityManager::GetEntity<TexCubemap>(const std::string& name_);

// ADD TO REGISTRY
template void EntityManager::AddEntity<MaterialHandler>(const std::string& name_, MaterialHandler* entity_);
template void EntityManager::AddEntity<Mesh>(const std::string& name_, Mesh* entity_);
template void EntityManager::AddEntity<ShaderProgram>(const std::string& name_, ShaderProgram* entity_);
template void EntityManager::AddEntity<Shader>(const std::string& name_, Shader* asset);
template void EntityManager::AddEntity<Tex2D>(const std::string& name_, Tex2D* entity_);
template void EntityManager::AddEntity<Tex2DArray>(const std::string& name_, Tex2DArray* entity_);
template void EntityManager::AddEntity<TexCubemap>(const std::string& name_, TexCubemap* entity_);

std::unique_ptr<EntityManager> EntityManager::engineInstance = nullptr;

EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {
	OnDestroy();
}

EntityManager* EntityManager::GetInstance()
{
	if (engineInstance.get() == nullptr) {
		engineInstance.reset(new EntityManager);
	}
	return engineInstance.get();
}

void EntityManager::OnDestroy() {
	for (auto it = map.begin(); it != map.end(); ++it) {
		delete (*it).second;
	}
	map.clear();
}

template<typename T>
void EntityManager::AddEntity(const std::string& name_, T* entity_) {
	map[name_] = (Entity*)entity_;
}

template<typename T>
T* EntityManager::GetEntity(const std::string& name_) {
	auto it = map.find(name_);
	if (it == map.end()) {
		return 0;
	}
	else {
		return (T*)((*it).second);
	}
}

bool EntityManager::StoredEntity(const std::string& name_) {
	return map.find(name_) != map.end();
}