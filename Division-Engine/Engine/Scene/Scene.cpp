#include "Scene.h"
#include "../Camera/ViewCamera.h"
#include "../Camera/TerrainCamera.h"
#include "../Core/Entity/EntityManager.h"
#include "../Graphic/Skybox.h"
#include "../Graphic/SunDirection.h"
#include "../Graphic/Terrain.h"
#include "../Rendering/Texture/TexCubemap.h"
#include "../Rendering/Texture/Tex2D.h"
#include "../Rendering/Texture/Tex2DArray.h"

Scene::Scene() {
	depthFBO = 0;
}

Scene::~Scene()
{
	delete depthFBO;
	OnDestroy();
}

void Scene::OnCreate()
{
	SetWindowProperties("Division Engine", 1280, 800);

	// SET THE SINGLETON GET INSTANCE HERE
	EntityManager* entityManager = EntityManager::GetInstance();

	// SKYBOX
	Skybox* skybox = new Skybox("Resources/Textures/SkyBoxCube/skycube.png");
	skyboxLoc = AddGameObject(skybox);

	// SUN DIRECTION
	Mesh* sunMesh = new Mesh();
	sunMesh->InitFromFile("Resources/Models/sphere.obj");
	entityManager->AddEntity("meshSphere", sunMesh);

	// SUN INIT SHADER
	Shader* vertSunlight = new Shader("Resources/Shaders/sunVert.vs");
	Shader* fragSunlight = new Shader("Resources/Shaders/sunFrag.fs");
	entityManager->AddEntity("sunVertShader", vertSunlight);
	entityManager->AddEntity("sunFragShader", fragSunlight);

	// SUN SHADER PROGRAM
	std::vector<Shader*> sunShaders;
	sunShaders.push_back(vertSunlight);
	sunShaders.push_back(fragSunlight);
	ShaderProgram* sunShaderProgram = new ShaderProgram(sunShaders);
	entityManager->AddEntity("sunShaderProgram", sunShaderProgram);

	// SUN TEXTURE
	Tex2D* sunTexture = new Tex2D();
	sunTexture->InitFromImageFile("Resources/Textures/moon.jpg");
	entityManager->AddEntity("sunTextures", sunTexture);

	// SUN MATERIAL HANDLER
	MaterialHandler* sunMaterial = new MaterialHandler();
	sunMaterial->SetTexture(0, sunTexture);
	sunMaterial->SetShaderProgram(0, sunShaderProgram);
	entityManager->AddEntity("sunMaterial", sunMaterial);

	// SUN ADD TO GAMEOBJECT
	SunDirection* sunDirection = new SunDirection(-1.0f, 0.6f, 0.0f);
	sunDirection->SetMesh(sunMesh);
	sunDirection->SetMaterial(sunMaterial);
	sunDirectionLoc = AddGameObject(sunDirection);
	AddEventHandler(sunDirection);
	

	// TERRAIN FEATURES
	// TERRAIN SHADER POINTERS
	Shader* terrainVS = new Shader("Resources/Shaders/terrainShader.vs");
	Shader* terrainTCS = new Shader("Resources/Shaders/terrainShader.tcs");
	Shader* terrainTES = new Shader("Resources/Shaders/terrainShader.tes");
	Shader* terrainGS = new Shader("Resources/Shaders/terrainShader.gs");
	Shader* terrainFS = new Shader("Resources/Shaders/terrainShader.fs");

	std::vector<Shader*> terrainShaderVector;
	terrainShaderVector.push_back(terrainVS);
	terrainShaderVector.push_back(terrainFS);
	terrainShaderVector.push_back(terrainTCS);
	terrainShaderVector.push_back(terrainTES);
	terrainShaderVector.push_back(terrainGS);
	ShaderProgram* terrainShaderProgramVector = new ShaderProgram(terrainShaderVector);

	// ADD TO REGISTRY FOR STORING DATA
	entityManager->AddEntity("terrainVS", terrainVS);
	entityManager->AddEntity("terrainFS", terrainFS);
	entityManager->AddEntity("terrainTCS", terrainTCS);
	entityManager->AddEntity("terrainTES", terrainTES);
	entityManager->AddEntity("terrainGS", terrainGS);
	entityManager->AddEntity("terrainShaderProgram", terrainShaderProgramVector);

	// TERRAIN DEPTH PASS SHADERS
	Shader* terrainDepthVS = new Shader("Resources/Shaders/terrainDepth.vs");
	Shader* terrainDepthTCS = new Shader("Resources/Shaders/terrainDepth.tcs");
	Shader* terrainDepthTES = new Shader("Resources/Shaders/terrainDepth.tes");

	std::vector<Shader*> terrainDepthShaderVector;
	terrainDepthShaderVector.push_back(terrainDepthVS);
	terrainDepthShaderVector.push_back(terrainDepthTCS);
	terrainDepthShaderVector.push_back(terrainDepthTES);
	ShaderProgram* terrainDepthShaderProgramVector = new ShaderProgram(terrainDepthShaderVector);
	
	entityManager->AddEntity("terrainDepthVS", terrainDepthVS);
	entityManager->AddEntity("terrainDepthTCS", terrainDepthTCS);
	entityManager->AddEntity("terrainDepthTES", terrainDepthTES);
	entityManager->AddEntity("terrainDepthShaderProgram", terrainDepthShaderProgramVector);


	// TERRAIN DISPLACEMENT TEXTURE
	// Init Height 16bit4 map
	Tex2D* terrainDis1 = new Tex2D();
	terrainDis1->InitFromImageFile("Resources/Textures/height16bit1.png", ImageFormatType::IMAGE_FORMAT_R16);
	terrainDis1->ModifyTextureParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainDis1->ModifyTextureParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	entityManager->AddEntity("terrainDis1", terrainDis1);

	Tex2D* terrainDis2 = new Tex2D();
	terrainDis2->InitFromImageFile("Resources/Textures/height16bit2.png", ImageFormatType::IMAGE_FORMAT_R16);
	terrainDis2->ModifyTextureParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainDis2->ModifyTextureParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	entityManager->AddEntity("terrainDis2", terrainDis2);

	Tex2D* terrainDis3 = new Tex2D();
	terrainDis3->InitFromImageFile("Resources/Textures/height16bit3.png", ImageFormatType::IMAGE_FORMAT_R16);
	terrainDis3->ModifyTextureParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainDis3->ModifyTextureParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	entityManager->AddEntity("terrainDis3", terrainDis3);


	// TERRAIN NORMAL MAPPING TEXTURE
	Tex2D* terrainNorm1 = new Tex2D();
	terrainNorm1->InitFromImageFile("Resources/Textures/normals1.png", ImageFormatType::IMAGE_FORMAT_RGB8_LINEAR);
	terrainNorm1->ModifyTextureParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainNorm1->ModifyTextureParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	entityManager->AddEntity("terrainNorm1", terrainNorm1);

	Tex2D* terrainNorm2 = new Tex2D();
	terrainNorm2->InitFromImageFile("Resources/Textures/normals2.png", ImageFormatType::IMAGE_FORMAT_RGB8_LINEAR);
	terrainNorm2->ModifyTextureParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainNorm2->ModifyTextureParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	entityManager->AddEntity("terrainNorm3", terrainNorm2);

	Tex2D* terrainNorm3 = new Tex2D();
	terrainNorm3->InitFromImageFile("Resources/Textures/normals3.png", ImageFormatType::IMAGE_FORMAT_RGB8_LINEAR);
	terrainNorm3->ModifyTextureParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainNorm3->ModifyTextureParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	entityManager->AddEntity("terrainNorm3", terrainNorm3);

	// SET TERRAIN MATERIAL TEXTURES INTO TEXTURE 2D ARRAY
	std::vector<std::string> terrainTex1;
	terrainTex1.push_back("Resources/Textures/1.png");
	terrainTex1.push_back("Resources/Textures/2.jpg");
	terrainTex1.push_back("Resources/Textures/3.jpg");
	terrainTex1.push_back("Resources/Textures/4.jpg");
	Tex2DArray* terrainTexArray1 = new Tex2DArray();
	terrainTexArray1->InitFromImageFiles(terrainTex1);
	entityManager->AddEntity("terrainTexArray1", terrainTexArray1);

	std::vector<std::string> terrainTex2;
	terrainTex2.push_back("Resources/Textures/6.jpg");
	terrainTex2.push_back("Resources/Textures/2.jpg");
	terrainTex2.push_back("Resources/Textures/8.jpg");
	terrainTex2.push_back("Resources/Textures/7.jpg");
	Tex2DArray* terrainTexArray2 = new Tex2DArray();
	terrainTexArray2->InitFromImageFiles(terrainTex2);
	entityManager->AddEntity("terrainTexArray2", terrainTexArray2);

	// COMBINE ALL TEXTURES INTO MATERIAL HANDLER
	MaterialHandler* terrainMatHandler1 = new MaterialHandler();
	terrainMatHandler1->SetShaderProgram(0, terrainShaderProgramVector);
	terrainMatHandler1->SetShaderProgram(1, terrainDepthShaderProgramVector);
	terrainMatHandler1->SetTexture(0, terrainDis1); // Height16bit4 map
	terrainMatHandler1->SetTexture(1, terrainNorm1); // Normal mapping texture
	terrainMatHandler1->SetTexture(2, terrainTexArray1);
	entityManager->AddEntity("terrainMat1", terrainMatHandler1);

	MaterialHandler* terrainMatHandler2 = new MaterialHandler();
	terrainMatHandler2->SetShaderProgram(0, terrainShaderProgramVector);
	terrainMatHandler2->SetShaderProgram(1, terrainDepthShaderProgramVector);
	terrainMatHandler2->SetTexture(0, terrainDis2); // Height16bit4 map
	terrainMatHandler2->SetTexture(1, terrainNorm2); // Normal mapping texture
	terrainMatHandler2->SetTexture(2, terrainTexArray2);
	entityManager->AddEntity("terrainMat2", terrainMatHandler2);

	MaterialHandler* terrainMatHandler3 = new MaterialHandler();
	terrainMatHandler3->SetShaderProgram(0, terrainShaderProgramVector);
	terrainMatHandler3->SetShaderProgram(1, terrainDepthShaderProgramVector);
	terrainMatHandler3->SetTexture(0, terrainDis3); // Height16bit4 map
	terrainMatHandler3->SetTexture(1, terrainNorm3); // Normal mapping texture
	terrainMatHandler3->SetTexture(2, terrainTexArray1);
	entityManager->AddEntity("terrainMat3", terrainMatHandler3);

	Terrain* terrain = new Terrain(sunDirection);
	terrain->SetMaterial(terrainMatHandler1);
	AddEventHandler(terrain);
	terrainLoc = AddGameObject(terrain);

	// DEPTH FBO
	InitFrameBufferObject();
	isShadowMapping = true;

	// TERRAIN CAMERA
	TerrainCamera* camera = new TerrainCamera();
	AddCamera(camera);
	SetActiveCamera(0);
	camera->SetProjectionMatrix(45.0f, (float)windowWidth / (float)windowHeight, 1.0f, 2048.0f);
	camera->SetInitPosAndRot(glm::vec3(0, 500, 0), glm::vec3(-65, 0, 0));

	//MISC
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glPolygonOffset(7.5f, 15.0f);

	CoreEngine::OnCreate();
}

void Scene::OnDestroy()
{
}

void Scene::InitFrameBufferObject()
{
	Tex2D* depthTex = EntityManager::GetInstance()->GetEntity<Tex2D>("depthTex");

	if (depthTex != 0) {
		delete depthTex;
	}

	depthTex = new Tex2D();
	depthTex->SetMipmapLevels(1);
	depthTex->InitFromImageData(GetWindowWidth() / 2, GetWindowHeight() / 2, 0, ImageFormatType::IMAGE_FORMAT_DEPTH_32F);
	depthTex->ModifyTextureParam(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	depthTex->ModifyTextureParam(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	MaterialHandler* m = EntityManager::GetInstance()->GetEntity<MaterialHandler>("terrainMat1");
	m->SetTexture(3, depthTex);
	m = EntityManager::GetInstance()->GetEntity<MaterialHandler>("terrainMat2");
	m->SetTexture(3, depthTex);
	m = EntityManager::GetInstance()->GetEntity<MaterialHandler>("terrainMat3");
	m->SetTexture(3, depthTex);

	EntityManager::GetInstance()->AddEntity("depthTex", depthTex);

	if (depthFBO == 0) {
		depthFBO = new FrameBufferObject();
	}

	depthFBO->AttachTextureToDepthBuffer(*depthTex);
	depthFBO->LoadGPU();
}

void Scene::Render()
{
	static const GLfloat backGroundColor[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
	static const GLfloat depthValue = 1.0f;

	if (isShadowMapping) {
		depthFBO->Bind();
		glEnable(GL_POLYGON_OFFSET_FILL);
		glClearBufferfv(GL_DEPTH, 0, &depthValue);
		gameObjects[terrainLoc]->Render(1);
		glDisable(GL_POLYGON_OFFSET_FILL);

		depthFBO->BindDefaultFramebuffer();
	}
	
	glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);

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

	// IF WINDOW RESIZE CALLBACKS INIT FBO TO ARRANGE DEPTH SHADER
	if (initialized) {
		InitFrameBufferObject();
	}

}

void Scene::KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_)
{
	CoreEngine::KeyCallback(key_, scanCode_, action_, mode_);

	if (key_ == GLFW_KEY_H && action_ == GLFW_PRESS) {
		isShadowMapping = !isShadowMapping;
	}
}

void Scene::CursorPositionCallback(const double xpos_, const double ypos_)
{
	CoreEngine::CursorPositionCallback(xpos_, ypos_);
}

void Scene::MouseButtonCallback(const int button_, const int action_, const int mode_)
{
	CoreEngine::mouseButtonCallback(button_, action_, mode_);
}
