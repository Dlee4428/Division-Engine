#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "../../../DivisionPCH.h"
#include "Entity.h"

class EntityManager
{
public:
	// Singleton design pattern to Intantiate functions for simply calling
	EntityManager(const EntityManager&) = delete;
	EntityManager(EntityManager&&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;
	EntityManager& operator=(EntityManager&&) = delete;

	static EntityManager* GetInstance();

	// Properly delete
	void OnDestroy();

	// Register Entity
	template<typename T>
	void AddEntity(const std::string& name_, T* entity_);
	bool StoredEntity(const std::string& name_);

	template<typename T>
	T* GetEntity(const std::string& name_);

private:
	EntityManager();
	~EntityManager();

	std::unordered_map<std::string, Entity*> map;

	static std::unique_ptr<EntityManager> engineInstance;
	friend std::default_delete<EntityManager>; // Whenever unique_ptr is out of scope this automatically call destructor
};

#endif // !ENTITYMANAGER_H
