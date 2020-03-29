#include "Skybox.h"
#include "../Core/Entity/EntityManager.h"
#include "../Core/CoreEngine.h"
#include "../Camera/Camera.h"

Skybox::Skybox(const std::string& texLoc_) : texLocation(texLoc_)
{
	// Declare Singleton Instance of Entity Manager
	EntityManager* entityManager = EntityManager::GetInstance();

	// Declaration of vert and frag shader pointers
	Shader* vertShader = new Shader("Resources/Shaders/skyboxVert.vs");
	Shader* fragShader = new Shader("Resources/Shaders/skyboxFrag.fs");
	entityManager->AddEntity("skyboxVert", vertShader);
	entityManager->AddEntity("skyboxFrag", fragShader);

	// Push vector of Shader pointer and combine them into shaderprogram
	std::vector<Shader*> skyboxShader;
	skyboxShader.push_back(vertShader);
	skyboxShader.push_back(fragShader);
	ShaderProgram* shaderProgram = new ShaderProgram(skyboxShader);
	entityManager->AddEntity("skyboxShaderProgram", shaderProgram);

	// Set yInverse of TexCubemap true
	TexCubemap* texCube = new TexCubemap(true);
	texCube->SetMipmapLevels(1);
	texCube->InitFromImageFile(texLoc_);
	entityManager->AddEntity("skyboxTexCube", texCube);

	// Set Material of texture and shaderProgram
	MaterialHandler* matHandler = new MaterialHandler();
	matHandler->SetTexture(0, texCube);
	matHandler->SetShaderProgram(0, shaderProgram);
	entityManager->AddEntity("skyboxMatHandler", matHandler);

	// Create Mesh Pointer to initialize Raw Data
	Mesh* mesh = new Mesh();
	mesh->InitFromRawData(8, 36); // Param -> Vert number and Indices number

	// Size of vertices position of float array
	GLfloat vPos[] = {
		-2.0f, 2.0f, 2.0f, // single Vertex point 
		-2.0f,-2.0f, 2.0f, 
		 2.0f,-2.0f, 2.0f, 
		 2.0f, 2.0f, 2.0f,
		-2.0f, 2.0f,-2.0f,
		-2.0f,-2.0f,-2.0f,  
		 2.0f,-2.0f,-2.0f, 
		 2.0f, 2.0f,-2.0f,
	};

	// Size of indices for skybox
	GLuint indices[] = {
		2, 1, 0,
		0, 3, 2,
		4, 5, 6,
		6, 7, 4,
		6, 2, 7,
		2, 3, 7,
		4, 1, 5,
		4, 0, 1,
		5, 1, 2,
		5, 2, 6,
		4, 3, 0,
		4, 7, 3
	};

	mesh->SetAttribute(0, vPos);
	mesh->SetIndices(indices);
	mesh->EndFromRawData();

	// From SceneObject
	SetMaterial(matHandler);
	SetMesh(mesh);
	entityManager->AddEntity("skyboxMesh", mesh);
}

Skybox::~Skybox()
{
}

void Skybox::OnCreate()
{
}

void Skybox::Render(int objectID_)
{
	material->Bind();

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(coreEngine->GetActiveCamera().GetProjectionMatrix()));		// PROJ MATRIX
	glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(coreEngine->GetActiveCamera().GetViewMatrix()));				// VIEW MATRIX
	glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(transform.GetTransformMatrix()));							// MODEL MATRIX

	glDisable(GL_DEPTH_TEST);
	mesh->RenderIndex();
	glEnable(GL_DEPTH_TEST);
}

void Skybox::Update(double deltaTime_)
{
	Camera& camera = coreEngine->GetActiveCamera();
	
	if (camera.LastUpdate()) {
		transform.SetPosition(camera.GetTransform().GetPosition(), WORLD);
	}
}
