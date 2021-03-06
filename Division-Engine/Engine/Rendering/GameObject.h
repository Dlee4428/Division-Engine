#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../../DivisionPCH.h"
#include "../Core/Event/EventHandler.h"
#include "3D/MaterialHandler.h"
#include "3D/Mesh.h"
#include "3D/Transform.h"

class CoreEngine;

class GameObject : public EventHandler {
public:
	GameObject();
	virtual ~GameObject();

	inline void SetMesh(Mesh* mesh_) { this->mesh = mesh_; }
	inline void SetMaterial(MaterialHandler* material_) { this->material = material_; }
	inline void SetCoreEngine(CoreEngine* core_) { coreEngine = core_; }

	virtual void OnCreate();
	virtual void Render(int objectID_ = 0);
	virtual void Update(double deltaTime_);

	inline Transform& GetTransform() { return transform; }
	inline MaterialHandler& GetMaterial() { return *material; }

	inline void GetRenderEnabled(bool e_) { renderEnabled = e_; }
	inline bool IsRenderEnabled() { return renderEnabled; }

protected:
	virtual void BindMaterial();

	CoreEngine* coreEngine;
	MaterialHandler* material;
	Mesh* mesh;

	Transform transform;

	bool renderEnabled;
};

#endif // !GAMEOBJECT_H
