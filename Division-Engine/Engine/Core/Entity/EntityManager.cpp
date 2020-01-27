#include "EntityManager.h"

class Material;
class Mesh;
class Texture2D;
class Texture2DArray;
class ShaderProgram;
class Shader;
class TextureCube;

template Material* EntityManager::GetEntity<Material>(const std::string& name_);
template Mesh* EntityManager::GetEntity<Mesh>(const std::string& name_);
template Texture2D* EntityManager::GetEntity<Texture2D>(const std::string& name_);
template TextureCube* EntityManager::GetEntity<TextureCube>(const std::string& name_);
template ShaderProgram* EntityManager::GetEntity<ShaderProgram>(const std::string& name_);

template void EntityManager::AddEntity<Material>(const std::string& name_, Material* entity_);
template void EntityManager::AddEntity<Mesh>(const std::string& name_, Mesh* entity_);
template void EntityManager::AddEntity<Texture2D>(const std::string& name_, Texture2D* entity_);
template void EntityManager::AddEntity<TextureCube>(const std::string& name_, TextureCube* entity_);
template void EntityManager::AddEntity<ShaderProgram>(const std::string& name_, ShaderProgram* entity_);
template void EntityManager::AddEntity<Shader>(const std::string& name_, Shader* asset);
template void EntityManager::AddEntity<TextureCube>(const std::string& name_, TextureCube* entity_);
template void EntityManager::AddEntity<Texture2DArray>(const std::string& name_, Texture2DArray* entity_);


std::unique_ptr<EntityManager> EntityManager::engineInstance = nullptr;

EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {
	OnDestroy();
}

EntityManager* EntityManager::GetInstance() {
	if (engineInstance.get() == nullptr) {
		engineInstance.reset(new EntityManager);
	}
	return engineInstance.get();
}

void EntityManager::OnDestroy() {
	for (auto it = map.begin(); it != map.end(); ++it)
		delete (*it).second;
	map.clear();
}

template<typename T>
void EntityManager::AddEntity(const std::string& name_, T* entity_) {
	map[name_] = (Entity*)entity_;
}

template<typename T>
T* EntityManager::GetEntity(const std::string& name_) {
	auto it = map.find(name_);
	if (it == map.end())
		return 0;
	else
		return (T*)((*it).second);
}

bool EntityManager::StoredEntity(const std::string& name_) {
	return map.find(name_) != map.end();
}