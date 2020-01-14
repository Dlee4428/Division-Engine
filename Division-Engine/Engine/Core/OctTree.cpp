#include "OctTree.h"

int OctNode::childNum = 0;

OctNode::OctNode(glm::vec3 position_, float size_, OctNode* parent_) :
	octBounds(nullptr), parent(nullptr), children(), objectList(std::vector<GameObject*>()), size(0.0f)
{
	octBounds = new BoundingBox();
	octBounds->minVert = position_;
	octBounds->maxVert = position_ + glm::vec3(size_);

	size = size_;

	for (int i = 0; i < 8; i++) {
		children[i] = nullptr;
	}
	parent = parent_;
}

OctNode::~OctNode()
{
	OnDestroy();
}

void OctNode::OnDestroy()
{
	delete octBounds;
	octBounds = nullptr;

	if (objectList.size() > 0) {
		for (auto m : objectList) {
			//		delete m;
			m = nullptr;
		}
		objectList.clear();
	}

	for (int i = 0; i < 8; i++) {
		if (children[i] != nullptr) {
			delete children[i];
			children[i] = nullptr;
		}
	}
}

void OctNode::Octify(int depth_)
{
	if (depth_ > 0 && this) {
		float half = size / 2.0f;

		// x left,  
		// x right + half
		// y top + half,  
		// y bottom
		// z front,  
		// z back + half
		// Shaked's
		children[OCT_TLF] = new OctNode(glm::vec3(octBounds->minVert.x, octBounds->minVert.y + half, octBounds->minVert.z + half), half, this);
		children[OCT_BLF] = new OctNode(glm::vec3(octBounds->minVert.x, octBounds->minVert.y, octBounds->minVert.z), half, this);
		children[OCT_BRF] = new OctNode(glm::vec3(octBounds->minVert.x + half, octBounds->minVert.y, octBounds->minVert.z), half, this);
		children[OCT_TRF] = new OctNode(glm::vec3(octBounds->minVert.x + half, octBounds->minVert.y + half, octBounds->minVert.z), half, this);
		children[OCT_TLR] = new OctNode(glm::vec3(octBounds->minVert.x, octBounds->minVert.y + half, octBounds->minVert.z + half), half, this);
		children[OCT_BLR] = new OctNode(glm::vec3(octBounds->minVert.x, octBounds->minVert.y, octBounds->minVert.z + half), half, this);
		children[OCT_BRR] = new OctNode(glm::vec3(octBounds->minVert.x + half, octBounds->minVert.y, octBounds->minVert.z + half), half, this);
		children[OCT_TRR] = new OctNode(glm::vec3(octBounds->minVert.x + half, octBounds->minVert.y + half, octBounds->minVert.z + half), half, this);

		// Homework
		childNum += 8;
	}
	if (depth_ > 0 && this) {
		for (int i = 0; i < 8; i++) {
			children[i]->Octify(depth_ - 1);
		}
	}
}

OctNode* OctNode::GetParent()
{
	return parent;
}

OctNode* OctNode::GetChild(OctChildren childPos_)
{
	return children[childPos_];
}

void OctNode::AddCollisionObject(GameObject* gameObject_)
{
	objectList.push_back(gameObject_);
}

int OctNode::GetObjectCount()
{
	return objectList.size();
}

bool OctNode::IsLeaf() const
{
	if (children[0] == nullptr) {
		return true;
	}
	return false;
}

BoundingBox* OctNode::GetBoundingBox() const
{
	return octBounds;
}

int OctNode::GetChildCount() const
{
	return childNum;
}


OctTree::OctTree(float worldSize_) : root(nullptr)
{
	root = new OctNode(glm::vec3(-worldSize_ / 2.0f), worldSize_, nullptr);
	root->Octify(3);
	std::cout << "There are " << root->GetChildCount() << " child nodes" << std::endl;
}

OctTree::~OctTree()
{
	OnDestroy();
}

void OctTree::OnDestroy()
{
	delete root;
	root = nullptr;

	if (rayIntersectionList.size() > 0) {
		for (auto cell : rayIntersectionList) {
			cell = nullptr;
		}
		rayIntersectionList.clear();
	}
}

void OctTree::AddObject(GameObject* obj_)
{
	AddObjectToCell(root, obj_);
}

GameObject* OctTree::GetCollision(Ray ray_)
{
	rayIntersectionList.clear();
	PrepareCollisionQuery(root, ray_);

	GameObject* hitResult = nullptr;
	float shortestDistance = FLT_MAX;

	for (auto& g : rayIntersectionList) {
		for (auto& node : g->objectList) {
			if (ray_.IsColliding(&node->GetBoundingBox())) {
				if (ray_.intersectionDistance < shortestDistance) {
					hitResult = node;
					shortestDistance = ray_.intersectionDistance;
				}
			}
		}
	}
	if (hitResult == nullptr) {
		return nullptr;
	}

	return hitResult;
}

void OctTree::AddObjectToCell(OctNode* cell_, GameObject* obj_)
{
	if (cell_) {
		BoundingBox nodeBox = *cell_->GetBoundingBox();
		if (nodeBox.Intersects(&obj_->GetBoundingBox())) {
			if (cell_->IsLeaf()) {
				cell_->AddCollisionObject(obj_);
				std::cout << "Added " << obj_->GetTag() << " to cell: " << glm::to_string(nodeBox.maxVert) << std::endl;
			}
			else {
				for (int i = 0; i < 8; i++) {
					AddObjectToCell(cell_->GetChild(static_cast<OctNode::OctChildren>(i)), obj_);
				}
			}
		}
	}
}

void OctTree::PrepareCollisionQuery(OctNode* cell_, Ray ray_)
{
	if (cell_) {
		if (ray_.IsColliding(cell_->GetBoundingBox())) {
			if (cell_->IsLeaf()) {
				rayIntersectionList.push_back(cell_);
			}
			else {
				for (int i = 0; i < 8; i++) {
					PrepareCollisionQuery(cell_->GetChild(static_cast<OctNode::OctChildren>(i)), ray_);
				}
			}
		}
	}
}
