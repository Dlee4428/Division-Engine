#include "SkyBox.h"
#include "../Rendering/3D/Mesh.h"
#include "../Rendering/3D/ObjLoader.h"
#include "../Rendering/3D/GameObject.h"
#include "../Core/Debug/Debug.h"
#include "../Camera/Camera.h"

SkyBox::SkyBox() : mode(0), textureID(0), textureSurface(nullptr),
meshPtr(nullptr), shaderPtr(nullptr), skyObject(nullptr),skyboxPtr(nullptr){}

SkyBox::~SkyBox() {
	glDeleteTextures(1, &textureID);
}

bool SkyBox::OnCreate()
{
	if (ObjLoader::loadOBJ("./Resources/Models/cube.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("./Resources/Shaders/skyBoxVert.glsl", "./Resources/Shaders/skyBoxFrag.glsl");
	skyboxPtr = new SkyBox();

	if (meshPtr == nullptr || shaderPtr == nullptr || skyboxPtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}
	if (skyboxPtr->LoadSkyBox(faces) == false) {
		Debug::Error("Cannot Load Skybox texture", __FILE__, __LINE__);
		return false;
	}
	skyObject = new GameObject(meshPtr, shaderPtr, skyboxPtr);
	if (skyObject == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	return true;
}

void SkyBox::OnDestroy()
{
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (skyObject) delete skyObject, skyObject = nullptr;
	if (skyboxPtr) delete skyboxPtr, skyboxPtr = nullptr;
}

void SkyBox::Update(const float deltaTime_) {
	static glm::mat4 skyModel;
	static float rot;
	rot += 0.0002f;

	skyObject->Update(deltaTime_);
	skyObject->setModelMatrix(glm::rotate(skyModel, rot, glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::translate(skyModel, glm::vec3(0.0f, 0.0f, 0.0f))
		* glm::scale(skyModel, glm::vec3(50.0f)));
}

void SkyBox::Render() const
{
	skyObject->Render();
}


// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
bool SkyBox::LoadSkyBox(const std::vector<std::string> faces) {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
	for (GLuint i = 0; i < faces.size(); i++) {	
		textureSurface = IMG_Load(faces[i].c_str());
		if (textureSurface == nullptr) {
			return false;
		}
		mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	SDL_FreeSurface(textureSurface);
	textureSurface = nullptr;
	return true;
}