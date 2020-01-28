#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::OnCreate()
{
	SetWindowProperties("Division Engine", 1280, 800);

	//MISC
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Scene::Render()
{
}

void Scene::Update(const double deltaTime_)
{
}

void Scene::WindowResizeCallback(const int width_, const int height_)
{
}

void Scene::KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_)
{
}

void Scene::CursorPositionCallback(const double xpos_, const double ypos_)
{
}

void Scene::MouseButtonCallback(const int button_, const int action_, const int mode_)
{
}

void Scene::InitFBO()
{
}
