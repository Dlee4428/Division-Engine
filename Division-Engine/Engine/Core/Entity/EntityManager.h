#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "../../../DivisionPCH.h"
#include "Entity.h"

class EntityManager
{
public:
	// This is Entity Repository
	// I'm using Lazy-evaluated singleton design pattern C++ 11 implementation
	// After Loading resources it will auto destroy
	// Singleton design pattern to Intantiate functions for simply calling
	// https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
	static EntityManager& GetInstance() {
		static EntityManager instance;
		return instance;
	}

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

public:
	EntityManager(EntityManager const&) = delete;
	void operator=(EntityManager const&) = delete;

// Note: Scott Meyers mentions in his Effective Modern
//       C++ book, that deleted functions should generally
//       be public as it results in better error messages
//       due to the compilers behavior to check accessibility
//       before deleted status
};

#endif // !ENTITYMANAGER_H
