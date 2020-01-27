#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "../../DivisionPCH.h"
#include "../Core/Event/EventHandler.h"
#include "3D/Material.h"
#include "3D/Mesh.h"
#include "3D/Transform.h"

class CoreEngine;

class SceneObject : public EventHandler {
public:
	SceneObject();
	virtual ~SceneObject();

	inline void SetMesh(Mesh* mesh_) { this->mesh = mesh_; }
	inline void SetMaterial(Material* material_) { this->material = material_; }
	inline void SetCoreEngine(CoreEngine* core_) { coreEngine = core_; }

	virtual void OnCreate();
	virtual void Render(int passIDref_ = 0);
	virtual void Update(double deltaTime_);

	inline Transform& GetTransform() { return transform; }
	inline Material& GetMaterial() { return *material; }

	inline void GetRenderEnabled(bool e_) { renderEnabled = e_; }
	inline bool IsRenderEnabled() { return renderEnabled; }

protected:
	virtual void BindMaterial();

	CoreEngine* coreEngine;
	Material* material;
	Mesh* mesh;

	Transform transform;

	bool renderEnabled;
};

#endif // !SCENEOBJECT_H
