#include "AssetManager.h"





AssetManager::AssetManager() {
}

AssetManager::~AssetManager() {
	OnDestroy();
}

void AssetManager::OnDestroy() {
	for (auto it = map.begin(); it != map.end(); ++it)
		delete (*it).second;
	map.clear();
}

template<typename T>
void AssetManager::AddAsset(const std::string& name, T* asset) {
	map[name] = (Asset*)asset;
}

template<typename T>
T* AssetManager::GetAsset(const std::string& name) {
	auto it = map.find(name);
	if (it == map.end())
		return 0;
	else
		return (T*)((*it).second);
}

bool AssetManager::HasAsset(const std::string& name) {
	return map.find(name) != map.end();
}