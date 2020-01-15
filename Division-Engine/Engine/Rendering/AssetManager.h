#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "../../DivisionPCH.h"
#include "Asset.h"

//Asset Repository
class AssetManager
{
public:
	static AssetManager& getInstance() {
		// Guaranteed to be lazy initialized
		// Guaranteed that it will be destroyed correctly
		static AssetManager instance;
		return instance;
	}

	void OnDestroy();

	template<typename T>
	void AddAsset(const std::string& name, T* asset);

	bool HasAsset(const std::string& name);

	template<typename T>
	T* GetAsset(const std::string& name);

private:
	AssetManager();
	~AssetManager();

	AssetManager(AssetManager const& copy);            // Not Implemented
	AssetManager& operator=(AssetManager const& copy); // Not Implemented

	std::unordered_map<std::string, Asset*> map;
};

#endif // !ASSETMANAGER_H
