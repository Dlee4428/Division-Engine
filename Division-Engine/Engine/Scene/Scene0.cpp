#include "Scene0.h"
#include "../Core/Window.h"
#include "../Core/Debug/Debug.h"
#include "../Camera/Camera.h"
#include "../Rendering/3D/GameObject.h"
#include "../Rendering/3D/Mesh.h"
#include "../Rendering/3D/ObjLoader.h"
#include "../Graphic/Shader.h"
#include "../Math/Noise.h"

Scene0::Scene0() :
	camera(nullptr),
	meshPtr(nullptr),
	shaderPtr(std::vector<Shader*>()),
	skyboxPtr(nullptr),
	lightSource(nullptr),
	deltaTime(0),
	noise(0){}

Scene0::~Scene0() {
}

bool Scene0::OnCreate() {
	camera = new Camera();
	shaderPtr.reserve(3);

	Noise perlinNoise;
	noise = perlinNoise.CreateNoise3D();
	perlinNoise.SetNoiseFrequency(50000);

	// Texture Creation
	if (ObjLoader::loadOBJ("Resources/Models/sphere.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr.push_back(new Shader("Resources/Shaders/reflectionVert.glsl", "Resources/Shaders/reflectionFrag.glsl"));
	skyboxPtr = new SkyBox();

	if (meshPtr == nullptr || shaderPtr.empty()) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}

	if (skyboxPtr->LoadSkyBox(skyboxPtr->faces) == false) {
		Debug::Error("Cannot Load Skybox texture", __FILE__, __LINE__);
		return false;
	}

	// Reflection pointers
	gameObjects.push_back(new GameObject(meshPtr, shaderPtr[0], skyboxPtr));
	gameObjects.push_back(new GameObject(meshPtr, shaderPtr[0], skyboxPtr));

	for (auto objects : gameObjects) {
		if (objects == nullptr) {
			Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
			return false;
		}
	}

	camera->OnCreate();

	// Allocating lightSource memory by assigned number
	lightSource = new glm::vec3[LIGHT_SOURCE_NUM];

	// Assign lightSource array of Vec3
	lightSource[1] = glm::vec3(-10.0, 20.0, 20.0);
	lightSource[2] = glm::vec3(10.0, -20.0, -20.0);

	return true;
}

void Scene0::OnDestroy() {
	if(camera) delete camera, camera = nullptr;
	if(meshPtr) delete meshPtr, meshPtr = nullptr;
	if(skyboxPtr) delete skyboxPtr, skyboxPtr = nullptr;

	if (shaderPtr.size() > 0) {
		for (auto& s : shaderPtr) {
			delete s;
			s = nullptr;
		}
		shaderPtr.clear();
	}

	// Free GameObject memory
	if (gameObjects.size() > 0) {
		for (auto g : gameObjects) {
			delete g;
			g = nullptr;
		}
		gameObjects.clear();
	}

	// Free lightSource memory
	if(lightSource) delete[] lightSource, lightSource = nullptr;
}

void Scene0::HandleEvents(const SDL_Event& sdlEvent) {
	camera->HandleEvents(sdlEvent);

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		switch (sdlEvent.key.keysym.sym) {
		case SDLK_1:
			chromatic = false;
			printf("\nRef mix shader");
			break;
		case SDLK_2:
			chromatic = true;
			printf("\nChromatic Shader");
			break;
		}
	}
}

void Scene0::Update(const float deltaTime_) {
	deltaTime += deltaTime_;

	for (GameObject* objects : gameObjects) {
		objects->Update(deltaTime_);
	}

	UpdateObjects();
	camera->Update(deltaTime_);
}

void Scene0::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GenerateUniforms();
	camera->Render();

	glUseProgram(0);
}

void Scene0::UpdateObjects()
{
	// ModelMatrix manipulations here
	static float rotation = 0.0f;
	rotation += 0.01f;
	static glm::mat4 model;

	// Orbiting Object
	gameObjects[0]->setModelMatrix(glm::rotate(model, rotation, glm::vec3(0.0, -5.0, 0.0))
		* glm::translate(model, glm::vec3(7.0f, 0.0f, 0.0f))
		* glm::scale(model, glm::vec3(0.5f)));

	gameObjects[1]->setModelMatrix(glm::rotate(model, rotation, glm::vec3(0.0, -5.0, 0.0))
		* glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f))
		* glm::scale(model, glm::vec3(1.0f)));
}

void Scene0::GenerateUniforms() const
{
	/// Draw your scene here
	for (GameObject* objects : gameObjects) {
		GLuint program = objects->getShader()->getProgram();
		glUseProgram(program);

		/// These pass the matricies and the light position to the GPU
		glUniformMatrix4fv(objects->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix())); // Projection Matrix uniform
		glUniformMatrix4fv(objects->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix())); // View Matrix uniform
		glUniform3fv(objects->getShader()->getUniformID("viewPos"), 2, glm::value_ptr(camera->getPosition())); // Camera Pos uniform

		glUniform1fv(objects->getShader()->getUniformID("time"), 1, &deltaTime); // Time uniform
		glUniform1uiv(objects->getShader()->getUniformID("pNoise"), 1, &noise);  // Noise uniform
		glUniform1i(objects->getShader()->getUniformID("chromatic"), chromatic); // Chromatic Light switch uniform

		objects->Render();
	}
}
