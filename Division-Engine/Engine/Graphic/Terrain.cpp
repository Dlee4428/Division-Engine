#include "Terrain.h"
#include "../Core/Entity/EntityManager.h"
#include "../Scene/Scene.h"

Terrain::Terrain(SunDirection* sunDirection_) : sunDirection(sunDirection_) {
}

Terrain::~Terrain() {
}

void Terrain::OnCreate() 
{
	// glPatchParameter — specifies the parameters for patch primitives
	// Source - https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glPatchParameter.xhtml
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	tessTriWidth = 20.0f;
	scaleDisplacement = 150.0f;
	initSize = glm::vec2(1000.0f, 1000.0f); // 1km width and height
	pixelGridSize = glm::ivec2(64, 64); // PixelGrid Size by 64 * 64, ivec2 = integers of vec2 instead of float
	patchCount = pixelGridSize.x * pixelGridSize.y;

	camera = &coreEngine->GetActiveCamera(); // Reference from Scene class CoreEngine

	// DETERMINING SUN PROJECTION MATRIX
	sunProjMat = glm::ortho(
		-initSize.x / 2.0f,								  // LEFT
		 initSize.x / 2.0f,								  // RIGHT
		-initSize.y / 2.0f,								  // BOTTOM
		 initSize.y / 2.0f,								  // TOP
		 sunDirection->sunDistance - initSize.x / 2.0f,   // Z NEAR
		 sunDirection->sunDistance + initSize.x / 2.0f);  // Z FAR

	// SHADOW MAPPING BIAS
	shadowBias = glm::mat4(
		 glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
		 glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
		 glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
		 glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	
	// DIRTY FLAGS SET UP BY INIT
	fogVisible = false;
	wireframeMode = false;
	texBasedOnHeight = true;

	// MINIMAP FROM TOP VIEW
	topViewMatrix = glm::lookAt(glm::vec3(0, 1500.0f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));

}


void Terrain::Render(int objectID_)
{
	glm::mat4 sunMatrix = sunProjMat * sunDirection->GetTransform().GetInverseTransformationMatrix();

	int width = coreEngine->GetWindowWidth();
	int height = coreEngine->GetWindowHeight();

	if (objectID_ == 0) {

		material->SetActiveShader(texBasedOnHeight ? 0 : 1);
		material->Bind();

		// UNIFORM LOCATIONS FOR TERRAIN
		// LOCATIONS STARTS FROM 31~ FOR TERRAIN LOCATIONS
		glUniform2i(31, pixelGridSize.x, pixelGridSize.y); 
		glUniform1f(32, scaleDisplacement);
		glUniform2f(33, initSize.x, initSize.y);
		glUniform1f(34, tessTriWidth);
		glUniform1i(35, wireframeMode);
		glUniform1i(36, ((Scene*)coreEngine)->enableShadowMapping);
		glUniform1i(37, fogVisible);

		// UNIFORM LOCATIONS FOR CAMERA
		// CAMERA LOCATIONS STARTS FROM 11~
		glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
		glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(shadowBias * sunMatrix));
		glUniform4fv(14, 6, camera->GetFrustum().GetPackedPlaneData());
		glUniform2i(15, width, height);

		// UNIFORM LOCATIONS FOR SUN
		glUniform3fv(30, 1, (GLfloat*)&sunDirection->GetDirectionLight().GetDirection());
		glUniform3fv(31, 1, (GLfloat*)&sunDirection->GetDirectionLight().GetColor());

		// GL PATCHES
		glDrawArraysInstanced(GL_PATCHES, 0, 4, patchCount);

		// TOP CAMERA FOR FRUSTUM CULLING VIEW
		glDisable(GL_DEPTH_TEST);
		int x = 0;
		int y = 0;
		int h = int((float)height / 3.0f);
		int w = int((float)h * (float)camera->GetFrustum().aspectRatio);

		glViewport(x, y, w, h);

		glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(topViewMatrix));
		glUniform2i(28, w, h);
		glDrawArraysInstanced(GL_PATCHES, 0, 4, patchCount);

		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, width, height);
	}
	else {
		// SHADOW MAPPING ENABLE
		material->SetActiveShader(2);
		material->Bind();

		// TERRAIN UNIFORM LOCATION BUT SHADOW MAPPING
		glUniform2i(31, pixelGridSize.x, pixelGridSize.y);
		glUniform1f(32, scaleDisplacement);
		glUniform2f(33, initSize.x, initSize.y);
		glUniform1f(34, tessTriWidth);

		// CAMERA UNIFORM LOCATION BUT SHADOW MAPPING
		glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
		glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(sunMatrix));
		glUniform2i(15, width, height);

		// INIT FOR DEPTH TEXTURE OF WIDTH AND HEIGHT AT GL VIEWPORT
		const ImageDataType& depthTexture = material->GetTextureHandler(3)->GetImageData(0);
		glViewport(0, 0, depthTexture.width, depthTexture.height);

		glDrawArraysInstanced(GL_PATCHES, 0, 4, patchCount);

		glViewport(0, 0, width, height);
	}
}

void Terrain::Update(double deltaTime_)
{
}

void Terrain::KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_)
{
}
