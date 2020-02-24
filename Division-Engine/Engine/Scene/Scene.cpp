#include "Scene.h"
#include "../Camera/ViewCamera.h"
#include "../Camera/TerrainCamera.h"
#include "../Core/Entity/EntityManager.h"
#include "../Graphic/Skybox.h"
#include "../Graphic/SunDirection.h"
#include "../Rendering/Texture/TexCubemap.h"
#include "../Rendering/Texture/Tex2D.h"

Scene::Scene() : depthFBO(0), enableShadowMapping(false), 
skyboxID(0), terrainID(0), sunDirectionID(0) {
	depthFBO = 0;
}

Scene::~Scene()
{
	OnDestroy();
}

void Scene::OnCreate()
{
	SetWindowProperties("Division Engine", 1280, 800);

	EntityManager& entityManager = EntityManager::GetInstance();

	///////////////////////////////////////////////////
	// Skybox
	Skybox* skybox = new Skybox("Resources/Textures/SkyBoxCube/skycube.png");
	skyboxID = AddGameObject(skybox);
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	// SunDirection
	Mesh* sunMesh = new Mesh();
	sunMesh->InitFromFile("Resources/Models/sphere.obj");
	entityManager.AddEntity("meshSphere", sunMesh);

	// Shaders for the Sunlight
	Shader* vertSunlight = new Shader("Resources/Shaders/sunVert.vs");
	Shader* fragSunlight = new Shader("Resources/Shaders/sunFrag.fs");

	std::vector<Shader*> sunShaders;
	sunShaders.push_back(vertSunlight);
	sunShaders.push_back(fragSunlight);
	ShaderProgram* sunShaderProgram = new ShaderProgram(sunShaders);
	entityManager.AddEntity("sunShaderProgram", sunShaderProgram);

	// Sun Texture
	Tex2D* sunTexture = new Tex2D();
	sunTexture->InitFromImageFile("Resources/Textures/earthclouds.jpg");
	entityManager.AddEntity("sunTextures", sunTexture);

	// Sun Material Handler
	MaterialHandler* sunMaterial = new MaterialHandler();
	sunMaterial->SetTexture(0, sunTexture);
	sunMaterial->SetShaderProgram(0, sunShaderProgram);
	entityManager.AddEntity("sunMaterial", sunMaterial);

	// Binding Sun
	SunDirection* sunDirection = new SunDirection(-1.0f, 0.6f, 0.0f);
	sunDirection->SetMesh(sunMesh);
	sunDirection->SetMaterial(sunMaterial);
	sunDirectionID = AddGameObject(sunDirection);
	AddEventHandler(sunDirection);
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	// Camera
	TerrainCamera* camera = new TerrainCamera();
	//ViewCamera* camera = new ViewCamera();
	AddCamera(camera);
	SetActiveCamera(0);
	camera->SetProjectionMatrix(45.0f, (float)windowWidth / (float)windowHeight, 1.0f, 2048.0f);
	camera->SetInitPosAndRot(glm::vec3(0, 500, 0), glm::vec3(-65, 0, 0));
	//camera->SetPositionAndLookAt(glm::vec3(5.0f, 500.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	//MISC
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glPolygonOffset(7.5f, 15.0f);

	CoreEngine::OnCreate();
	///////////////////////////////////////////////////
}

void Scene::OnDestroy()
{
	delete depthFBO;
}

void Scene::Render()
{
	//static const GLfloat bgColor[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
	//static const GLfloat depthValue = 1.0f;

	
	//glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);

	CoreEngine::Render();
}

void Scene::Update(const double deltaTime_)
{
	TerrainCamera& camera = ((TerrainCamera&)GetActiveCamera());


	CoreEngine::Update(deltaTime_);
}

void Scene::WindowResizeCallback(const int width_, const int height_)
{
	CoreEngine::WindowResizeCallback(width_, height_);

}

void Scene::KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_)
{
	CoreEngine::KeyCallback(key_, scanCode_, action_, mode_);
}

void Scene::CursorPositionCallback(const double xpos_, const double ypos_)
{
	CoreEngine::CursorPositionCallback(xpos_, ypos_);
}

void Scene::MouseButtonCallback(const int button_, const int action_, const int mode_)
{
	CoreEngine::mouseButtonCallback(button_, action_, mode_);
}

void Scene::InitFBO()
{
}
