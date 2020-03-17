#include "Terrain.h"
#include "../Core/Entity/EntityManager.h"
#include "../Scene/Scene.h"

Terrain::Terrain(SunDirection* sunDirection_) : sunDirection(sunDirection_) {
}

Terrain::~Terrain() {
}

void Terrain::OnCreate() 
{
	// glPatchParameter � specifies the parameters for patch primitives
	// Source - https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glPatchParameter.xhtml
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	scaleDisplacement = 150.0f;
	tessTriWidth = 20.0f;
	patchSize = glm::ivec2(64, 64); // PixelGrid Size by 64 * 64, ivec2 = integers of vec2 instead of float
	worldSize = glm::vec2(1000.0f, 1000.0f); // 1km width and height
	patchCount = patchSize.x * patchSize.y;

	camera = &coreEngine->GetActiveCamera(); // Reference from Scene class CoreEngine

	// DETERMINING SUN PROJECTION MATRIX
	sunProjMat = glm::ortho(
		-worldSize.x / 2.0f,								  // LEFT
		 worldSize.x / 2.0f,								  // RIGHT
		-worldSize.y / 2.0f,								  // BOTTOM
		 worldSize.y / 2.0f,								  // TOP
		 sunDirection->sunDistance - worldSize.x / 2.0f,   // Z NEAR
		 sunDirection->sunDistance + worldSize.x / 2.0f);  // Z FAR

	// SHADOW MAPPING BIAS
	shadowBias = glm::mat4(
		 glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
		 glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
		 glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
		 glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	
	// DIRTY FLAGS SET UP BY INIT
	fogVisible = false;
	wireframeMode = false;

	// MINIMAP FROM TOP VIEW
	topViewMatrix = glm::lookAt(glm::vec3(0, 1400.0f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));

}


void Terrain::Render(int objectID_)
{
	glm::mat4 sunMatrix = sunProjMat * sunDirection->GetTransform().GetInvTransformMatrix();

	int width = coreEngine->GetWindowWidth();
	int height = coreEngine->GetWindowHeight();

	if (objectID_ == 0) {

		material->SetActiveShader(0);
		material->Bind();

		// UNIFORM LOCATIONS FOR CAMERA
		// CAMERA LOCATIONS STARTS FROM 11~
		glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(shadowBias * sunMatrix));		// Shadow Mapping with Sun Directional Light
		glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));		// Camera View Matrix Location
		glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix())); // Camera Projection Matrix Location
		glUniform4fv(14, 6, camera->GetFrustum().GetFrustumPlanes());						// Sending frustum planes to TCS shader

		// UNIFORM LOCATIONS FOR SUN
		glUniform3fv(20, 1, (GLfloat*)&sunDirection->GetDirectionLight().GetDirection());	// Sun Directional Light -> Directions
		glUniform3fv(21, 1, (GLfloat*)&sunDirection->GetDirectionLight().GetColor());		// Sun Directional Light -> Colors    

		// UNIFORM LOCATIONS FOR TERRAIN
		// LOCATIONS STARTS FROM 31~ FOR TERRAIN LOCATIONS
		glUniform2i(30, width, height);														// Viewport Size Location					TCS, GS
		glUniform2i(31, patchSize.x, patchSize.y);											// Patch Grid Size Location					VS
		glUniform1f(32, scaleDisplacement);													// Scale Displacement Location				TCS, TES, FS
		glUniform1f(33, tessTriWidth);														// Tessellation Triangle Width Location		TCS
		glUniform2f(34, worldSize.x, worldSize.y);											// World Size Location 1km					VS
		glUniform1i(35, wireframeMode);														// WireframeMode Bool Location				FS
		glUniform1i(36, ((Scene*)coreEngine)->isShadowMapping);								// Shadow Mapping Bool Location				FS
		glUniform1i(37, fogVisible);														// Fog Bool Location						FS
	
		// GL PATCHES
		glDrawArraysInstanced(GL_PATCHES, 0, 4, patchCount);

		// TOP CAMERA FOR FRUSTUM CULLING VIEW
		glDisable(GL_DEPTH_TEST);
		int x = 0;
		int y = 0;
		int h = int((float)height / 3.0f);
		int w = int((float)h * (float)camera->GetFrustum().aspectRatio);

		glViewport(x, y, w, h);

		glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(topViewMatrix));					// Using View Matrix Location
		glUniform2i(30, w, h);																// Viewport size Location
		glDrawArraysInstanced(GL_PATCHES, 0, 4, patchCount);

		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, width, height);
	}
	else {
		// TERRAIN DEPTH PASS
		material->SetActiveShader(1);
		material->Bind();

		// CAMERA UNIFORM LOCATION BUT DEPTH PASS
		glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(sunMatrix));
		glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

		// TERRAIN UNIFORM LOCATION BUT DEPTH PASS
		glUniform2i(30, width, height);
		glUniform2i(31, patchSize.x, patchSize.y);
		glUniform1f(32, scaleDisplacement);
		glUniform1f(33, tessTriWidth);
		glUniform2f(34, worldSize.x, worldSize.y);

		// INIT FOR DEPTH TEXTURE OF WIDTH AND HEIGHT AT GL VIEWPORT
		const ImageDataType& depthTex = material->GetTextureHandler(3)->GetImageData(0);
		glViewport(0, 0, depthTex.width, depthTex.height);

		glDrawArraysInstanced(GL_PATCHES, 0, 4, patchCount);
		glViewport(0, 0, width, height);
	}
}

void Terrain::Update(double deltaTime_)
{
}

void Terrain::KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_)
{
	if (key_ == GLFW_KEY_O) {
		scaleDisplacement -= scaleDisplacement * 0.03f;
	}
	else if (key_ == GLFW_KEY_P) {
		scaleDisplacement += scaleDisplacement * 0.03f;
	}

	else if (key_ == GLFW_KEY_K) {
		tessTriWidth = glm::max(tessTriWidth - 1.0f, 0.0f);
	}
	else if (key_ == GLFW_KEY_L) {
		tessTriWidth += 1.0f;
	}

	else if (key_ == GLFW_KEY_G && action_ == GLFW_PRESS) {
		wireframeMode = !wireframeMode;
	}
	else if (key_ == GLFW_KEY_F && action_ == GLFW_PRESS) {
		fogVisible = !fogVisible;
	}

	else if (key_ == GLFW_KEY_1 && action_ == GLFW_PRESS) {
		MaterialHandler* m = EntityManager::GetInstance()->GetEntity<MaterialHandler>("terrainMat1");
		SetMaterial(m);
	}
	else if (key_ == GLFW_KEY_2 && action_ == GLFW_PRESS) {
		MaterialHandler* m = EntityManager::GetInstance()->GetEntity<MaterialHandler>("terrainMat2");
		SetMaterial(m);
	}
	else if (key_ == GLFW_KEY_3 && action_ == GLFW_PRESS) {
		MaterialHandler* m = EntityManager::GetInstance()->GetEntity<MaterialHandler>("terrainMat3");
		SetMaterial(m);
	}
}
