#include "Scene.h"
#include "../Camera/ViewCamera.h"
#include "../Core/Entity/EntityManager.h"
#include "../Graphic/Skybox.h"
#include "../Graphic/SunDirection.h"
#include "../Graphic/Water.h"
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
	///////////////////////////////////////////////////////////////////////////
	// SET THE SINGLETON GET INSTANCE HERE
	EntityManager* entityManager = EntityManager::GetInstance();
	///////////////////////////////////////////////////////////////////////////
	// TERRAIN CAMERA
	terrainCamera = new TerrainCamera();
	AddCamera(terrainCamera);
	SetActiveCamera(0);
	terrainCamera->SetProjectionMatrix(45.0f, (float)windowWidth / (float)windowHeight, 1.0f, 2048.0f);
	terrainCamera->SetInitPosAndRot(glm::vec3(0, 500, 0), glm::vec3(-65, 0, 0));

	///////////////////////////////////////////////////////////////////////////
	// SKYBOX
	Skybox* skybox = new Skybox("Resources/Textures/SkyBoxCube/skycube.png");
	skyboxLoc = AddGameObject(skybox);

	///////////////////////////////////////////////////////////////////////////
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
	///////////////////////////////////////////////////////////////////////
	//// WATER
	//Mesh* waterMesh = new Mesh();
	//waterMesh->InitFromFile("Resources/Models/plane.obj");
	//entityManager->AddEntity("meshPlane", waterMesh);

	//// WATER INIT SHADER
	//Shader* vertWater = new Shader("Resources/Shaders/waterShader.vs");
	//Shader* fragWater = new Shader("Resources/Shaders/waterShader.fs");
	//entityManager->AddEntity("waterVertShader", vertWater);
	//entityManager->AddEntity("waterFragShader", fragWater);

	//// WATER SHADER PROGRAM
	//std::vector<Shader*> waterShaders;
	//waterShaders.push_back(vertWater);
	//waterShaders.push_back(fragWater);
	//ShaderProgram* waterShaderProgram = new ShaderProgram(waterShaders);
	//entityManager->AddEntity("waterShaderProgram", waterShaderProgram);

	//// WATER TEXTURE
	//
	//// WATER MATERIAL HANDLER
	//MaterialHandler* waterMaterial = new MaterialHandler();
	////waterMaterial->SetTexture(0, reflectionTexture);
	////waterMaterial->SetTexture(1, refractionTexture);
	//waterMaterial->SetShaderProgram(0, waterShaderProgram);
	//entityManager->AddEntity("waterMaterial", waterMaterial);

	//// WATER ADD TO GAMEOBJECT
	//Water* water = new Water(sunDirection);
	//water->SetMesh(waterMesh);
	//water->SetMaterial(waterMaterial);
	//waterLoc = AddGameObject(water);
	//AddEventHandler(water);
	////////////////////////////////////////////////////////////////////////
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
	///////////////////////////////////////////////////////////////////////////
	// ADD TO REGISTRY FOR STORING DATA
	entityManager->AddEntity("terrainVS", terrainVS);
	entityManager->AddEntity("terrainFS", terrainFS);
	entityManager->AddEntity("terrainTCS", terrainTCS);
	entityManager->AddEntity("terrainTES", terrainTES);
	entityManager->AddEntity("terrainGS", terrainGS);
	entityManager->AddEntity("terrainShaderProgram", terrainShaderProgramVector);
	///////////////////////////////////////////////////////////////////////////
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
	///////////////////////////////////////////////////////////////////////////
	// TERRAIN DISPLACEMENT TEXTURE
	// Init Height 16bit4 map
	Tex2D* terrainDis1 = new Tex2D();
	terrainDis1->InitFromImageFile("Resources/Textures/heightmap16bit1.png", ImageFormatType::IMAGE_FORMAT_R16);
	terrainDis1->ModifyTextureParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainDis1->ModifyTextureParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	entityManager->AddEntity("terrainDis1", terrainDis1);

	Tex2D* terrainDis2 = new Tex2D();
	terrainDis2->InitFromImageFile("Resources/Textures/heightmap16bit4.png", ImageFormatType::IMAGE_FORMAT_R16);
	terrainDis2->ModifyTextureParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainDis2->ModifyTextureParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	entityManager->AddEntity("terrainDis2", terrainDis2);

	Tex2D* terrainDis3 = new Tex2D();
	terrainDis3->InitFromImageFile("Resources/Textures/heightmap16bit3.png", ImageFormatType::IMAGE_FORMAT_R16);
	terrainDis3->ModifyTextureParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainDis3->ModifyTextureParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	entityManager->AddEntity("terrainDis3", terrainDis3);
	///////////////////////////////////////////////////////////////////////////
	// TERRAIN NORMAL MAPPING TEXTURE
	Tex2D* terrainNorm1 = new Tex2D();
	terrainNorm1->InitFromImageFile("Resources/Textures/normalmap1.png", ImageFormatType::IMAGE_FORMAT_RGB8_LINEAR);
	terrainNorm1->ModifyTextureParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainNorm1->ModifyTextureParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	entityManager->AddEntity("terrainNorm1", terrainNorm1);

	Tex2D* terrainNorm2 = new Tex2D();
	terrainNorm2->InitFromImageFile("Resources/Textures/normalmap2.png", ImageFormatType::IMAGE_FORMAT_RGB8_LINEAR);
	terrainNorm2->ModifyTextureParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainNorm2->ModifyTextureParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	entityManager->AddEntity("terrainNorm3", terrainNorm2);

	Tex2D* terrainNorm3 = new Tex2D();
	terrainNorm3->InitFromImageFile("Resources/Textures/normalmap3.png", ImageFormatType::IMAGE_FORMAT_RGB8_LINEAR);
	terrainNorm3->ModifyTextureParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainNorm3->ModifyTextureParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	entityManager->AddEntity("terrainNorm3", terrainNorm3);
	///////////////////////////////////////////////////////////////////////////
	// SET TERRAIN MATERIAL TEXTURES INTO TEXTURE 2D ARRAY
	std::vector<std::string> terrainTex1;
	terrainTex1.push_back("Resources/Textures/1grass.png");
	terrainTex1.push_back("Resources/Textures/3soil.jpg");
	terrainTex1.push_back("Resources/Textures/8wetGrass.jpg");
	terrainTex1.push_back("Resources/Textures/4rock.jpg");
	Tex2DArray* terrainTexArray1 = new Tex2DArray();
	terrainTexArray1->InitFromImageFiles(terrainTex1);
	entityManager->AddEntity("terrainTexArray1", terrainTexArray1);

	std::vector<std::string> terrainTex2;
	terrainTex2.push_back("Resources/Textures/5grassRock.jpg");
	terrainTex2.push_back("Resources/Textures/6hardSoil.jpg");
	terrainTex2.push_back("Resources/Textures/2grassySoil.jpg");
	terrainTex2.push_back("Resources/Textures/7deadGrass.jpg");
	Tex2DArray* terrainTexArray2 = new Tex2DArray();
	terrainTexArray2->InitFromImageFiles(terrainTex2);
	entityManager->AddEntity("terrainTexArray2", terrainTexArray2);
	///////////////////////////////////////////////////////////////////////////
	// COMBINE ALL TEXTURES INTO MATERIAL HANDLER
	MaterialHandler* terrainMatHandler1 = new MaterialHandler();
	terrainMatHandler1->SetShaderProgram(0, terrainShaderProgramVector);
	terrainMatHandler1->SetShaderProgram(1, terrainDepthShaderProgramVector);
	terrainMatHandler1->SetTexture(0, terrainDis1); // Height16bit4 map
	terrainMatHandler1->SetTexture(1, terrainNorm1); // Normal mapping texture
	terrainMatHandler1->SetTexture(2, terrainTexArray1); // Texture array blending
	entityManager->AddEntity("terrainMat1", terrainMatHandler1);

	MaterialHandler* terrainMatHandler2 = new MaterialHandler();
	terrainMatHandler2->SetShaderProgram(0, terrainShaderProgramVector);
	terrainMatHandler2->SetShaderProgram(1, terrainDepthShaderProgramVector);
	terrainMatHandler2->SetTexture(0, terrainDis2); // Height16bit4 map
	terrainMatHandler2->SetTexture(1, terrainNorm2); // Normal mapping texture
	terrainMatHandler2->SetTexture(2, terrainTexArray2); // Texture array blending
	entityManager->AddEntity("terrainMat2", terrainMatHandler2);

	MaterialHandler* terrainMatHandler3 = new MaterialHandler();
	terrainMatHandler3->SetShaderProgram(0, terrainShaderProgramVector);
	terrainMatHandler3->SetShaderProgram(1, terrainDepthShaderProgramVector);
	terrainMatHandler3->SetTexture(0, terrainDis3); // Height16bit4 map
	terrainMatHandler3->SetTexture(1, terrainNorm3); // Normal mapping texture
	terrainMatHandler3->SetTexture(2, terrainTexArray1); // Texture array blending
	entityManager->AddEntity("terrainMat3", terrainMatHandler3);

	Terrain* terrain = new Terrain(sunDirection);
	terrain->SetMaterial(terrainMatHandler1);
	AddEventHandler(terrain);
	terrainLoc = AddGameObject(terrain);
	///////////////////////////////////////////////////////////////////////////
	// DEPTH FBO
//	InitWaterFrameBuffer();
	InitFrameBufferObject();
	isShadowMapping = true;
	///////////////////////////////////////////////////////////////////////////
	////MISC
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glCullFace — specify whether front- or back-facing polygons can be culled
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // BY SETTING GL_FRONT OR BACK, this determine where to culled

	glPolygonOffset(7.5f, 15.0f);

	CoreEngine::OnCreate();
}

void Scene::OnDestroy()
{
}

void Scene::InitFrameBufferObject()
{
	Tex2D* depthTerrain = EntityManager::GetInstance()->GetEntity<Tex2D>("depthTex");

	if (depthTerrain != 0) {
		delete depthTerrain;
	}

	depthTerrain = new Tex2D();
	depthTerrain->SetMipmapLevels(1);
	// DEFAULT IS SRGB8
	depthTerrain->InitFromImageData(GetWindowWidth() / 2, GetWindowHeight() / 2, 0, ImageFormatType::IMAGE_FORMAT_DEPTH_32F);
	depthTerrain->ModifyTextureParam(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	depthTerrain->ModifyTextureParam(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	MaterialHandler* matHandler = EntityManager::GetInstance()->GetEntity<MaterialHandler>("terrainMat1");
	matHandler->SetTexture(3, depthTerrain);
	matHandler = EntityManager::GetInstance()->GetEntity<MaterialHandler>("terrainMat2");
	matHandler->SetTexture(3, depthTerrain);
	matHandler = EntityManager::GetInstance()->GetEntity<MaterialHandler>("terrainMat3");
	matHandler->SetTexture(3, depthTerrain);

	EntityManager::GetInstance()->AddEntity("depthTex", depthTerrain);

	if (depthFBO == 0) {
		depthFBO = new FrameBufferObject();
	}

	depthFBO->AttachTextureToDepthBuffer(*depthTerrain);
	depthFBO->Process();
}

void Scene::InitWaterFrameBuffer()
{
	//	Tex2D* refractionTexture = EntityManager::GetInstance()->GetEntity<Tex2D>("refractionTex");

	//if (refractionTexture != 0) {
	//	delete refractionTexture;
	//}
	// SINGLETON GET INSTANCE OF BOTH REFLECTION AND REFRACTION TEXTURE
	Tex2D* reflectionTexture = EntityManager::GetInstance()->GetEntity<Tex2D>("reflectionTex");

	// FREE MEMORY if Ref and refraction Texture pointer is not equal to 0
	if (reflectionTexture != 0) {
		delete reflectionTexture;
	}
	
	reflectionTexture = new Tex2D();
	reflectionTexture->SetMipmapLevels(1);
	reflectionTexture->InitFromImageData(1024, 768, 0);

	MaterialHandler* reflectionHandler = EntityManager::GetInstance()->GetEntity<MaterialHandler>("terrainMat1");
	reflectionHandler->SetTexture(4, reflectionTexture);
	reflectionHandler = EntityManager::GetInstance()->GetEntity<MaterialHandler>("terrainMat2");
	reflectionHandler->SetTexture(4, reflectionTexture);
	reflectionHandler = EntityManager::GetInstance()->GetEntity<MaterialHandler>("terrainMat3");
	reflectionHandler->SetTexture(4, reflectionTexture);

	EntityManager::GetInstance()->AddEntity("reflectionTex", reflectionTexture);

	if (waterFBO == 0) {
		waterFBO = new WaterFrameBuffer();
	}

	waterFBO->AttachTextureToColorBuffer(0, *reflectionTexture);
	waterFBO->Process();
}

void Scene::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Black background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static const GLfloat depthOffset = 1.0f;

	// WATER QUADS
	//waterFBO->Bind();
	//waterFBO->UnBindFrameBuffer();

	if (isShadowMapping) {		
		depthFBO->Bind();  //Frame buffer BIND here MASTER RENDERER
		glEnable(GL_POLYGON_OFFSET_FILL);
		glClearBufferfv(GL_DEPTH, 0, &depthOffset);
		gameObjects[terrainLoc]->Render(1); // int Object ID location at Render() = 0 is default
		glDisable(GL_POLYGON_OFFSET_FILL);
		depthFBO->UnBindFrameBuffer();
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
