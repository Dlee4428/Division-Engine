#ifndef TERRAIN_H
#define TERRAIN_H


#include "../../DivisionPCH.h"
#include "../Camera/Camera.h"

class Terrain {
public:
	Terrain();
	~Terrain();

	void OnCreate();
	void Update(const float deltaTime_);
	void Render();
private:
	float displacementScale;
	float tessellatedTriWidth;
	glm::ivec2 gridSize;
	glm::vec2 size;
	int patchCount;

	Camera* camera;
	//Sun* sun;

	glm::mat4 topViewMatrix;

	bool drawWireframe;
	bool enableFog;
	bool heightBasedTexturing;

	//depth pass stuff
	glm::mat4 sunProjectionMatrix;
	glm::mat4 bias;
};

#endif // !TERRAIN_H
