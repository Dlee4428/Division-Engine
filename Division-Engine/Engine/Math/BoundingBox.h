#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "../../DivisionPCH.h"

struct BoundingBox {
	glm::vec3 minVert, maxVert;
	glm::mat4 transform;

	inline BoundingBox() {
		minVert = glm::vec3(FLT_MAX);
		maxVert = glm::vec3(FLT_MIN);
		transform = glm::mat4();
	}

	inline BoundingBox(glm::vec3 minVert_, glm::vec3 maxVert_, glm::mat4 transform_) {
		minVert = minVert_;
		maxVert = maxVert_;
		transform = transform_;
	}

	// This function should first start by transforming the minand max verts of both boxes
	// Then check to see if the two boxes are colliding, use AABB collision detection
	inline bool Intersects(BoundingBox* box_) {
		glm::vec3 minCorner = GetTransformedPoint(minVert, transform);
		glm::vec3 maxCorner = GetTransformedPoint(maxVert, transform);

		glm::vec3 otherMinCorner = GetTransformedPoint(box_->minVert, box_->transform);
		glm::vec3 otherMaxCorner = GetTransformedPoint(box_->maxVert, box_->transform);

		// AABB collision here
		if (maxCorner.x < otherMinCorner.x || minCorner.x > otherMaxCorner.x) return false;
		if (maxCorner.y < otherMinCorner.y || minCorner.y > otherMaxCorner.y) return false;
		if (maxCorner.z < otherMinCorner.z || minCorner.z > otherMaxCorner.z) return false;

		return true;
	}

	inline glm::vec3 GetTransformedPoint(glm::vec3 point_, glm::mat4 transform_) {
		return glm::vec3(transform_[3].x, transform_[3].y, transform_[3].z) + point_;
	}
};
#endif // !BOUNDINGBOX_H
