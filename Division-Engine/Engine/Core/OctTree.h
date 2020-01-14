#ifndef OCTTREE_H
#define OCTTREE_H

#include "../Rendering/3D/GameObject.h"
#include "../Math/Ray.h"

class OctNode {
public:
	enum OctChildren {
		OCT_TLF,
		OCT_BLF,
		OCT_BRF,
		OCT_TRF,
		OCT_TLR,
		OCT_BLR,
		OCT_BRR,
		OCT_TRR
	};
	OctNode(glm::vec3 position_, float size_, OctNode* parent_);
	~OctNode();
	void OnDestroy();
	void Octify(int depth_);
	OctNode* GetParent();
	OctNode* GetChild(OctChildren childPos_);
	void AddCollisionObject(GameObject* gameObject_);
	int GetObjectCount();
	bool IsLeaf() const;
	BoundingBox* GetBoundingBox() const;
	int GetChildCount() const;
private:
	friend class OctTree;
	BoundingBox* octBounds;
	OctNode* parent;
	OctNode* children[8];
	std::vector<GameObject*> objectList;
	float size;
	static int childNum;
};

class OctTree
{
public:
	OctTree(float worldSize_);
	~OctTree();

	void OnDestroy();
	void AddObject(GameObject* obj_);
	GameObject* GetCollision(Ray ray_);
private:
	OctNode* root;
	std::vector<OctNode*> rayIntersectionList;
	void AddObjectToCell(OctNode* cell_, GameObject* obj_);
	void PrepareCollisionQuery(OctNode* cell_, Ray ray_);
};


#endif // !OCTTREE_H
