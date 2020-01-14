#include "MouseEventListener.h"
#include "../Core//CoreEngine.h"

CoreEngine* MouseEventListener::engineInstance = nullptr;
int MouseEventListener::mouseX = 0;
int MouseEventListener::mouseY = 0;
int MouseEventListener::prevMouseX = 0;
int MouseEventListener::prevMouseY = 0;
bool MouseEventListener::firstUpdate = true;

MouseEventListener::~MouseEventListener()
{

}

void MouseEventListener::RegisterEngineObject(CoreEngine* engine_)
{
	engineInstance = engine_;
}

void MouseEventListener::Update(SDL_Event e_)
{
	switch (e_.type) {
	case SDL_MOUSEBUTTONDOWN:
		UpdateMousePos();
		NotifyMousePressed(e_.button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		UpdateMousePos();
		NotifyMouseReleased(e_.button.button);
		break;
	case SDL_MOUSEMOTION:
		UpdateMousePos();
		NotifyMouseMove();
		break;
	case SDL_MOUSEWHEEL:
		NotifyMouseScroll(e_.wheel.y);
		break;
	default:
		break;
	}
}

void MouseEventListener::NotifyMousePressed(int buttonType_)
{
	if (engineInstance) {
		engineInstance->NotifyMousePressed(mouseX, mouseY);
	}
}

void MouseEventListener::NotifyMouseReleased(int buttonType_)
{
	if (engineInstance) {
		engineInstance->NotifyMouseReleased(mouseX, mouseY, buttonType_);
	}
}

void MouseEventListener::NotifyMouseMove()
{
	if (engineInstance) {
		engineInstance->NotifyMouseMove(mouseX, mouseY);
	}
}

void MouseEventListener::NotifyMouseScroll(int y_)
{
	if (engineInstance) {
		engineInstance->NotifyMouseScroll(y_);
	}
}

glm::vec2 MouseEventListener::GetPreviousMousePos()
{
	return glm::vec2(prevMouseX, prevMouseY);
}

glm::vec2 MouseEventListener::GetMousePos()
{
	return glm::vec2(mouseX, mouseY);
}

glm::vec2 MouseEventListener::GetMouseOffset()
{
	return glm::vec2(mouseX - prevMouseX, prevMouseY - mouseY);
}

void MouseEventListener::UpdateMousePos()
{
	int tempX, tempY;
	SDL_GetMouseState(&tempX, &tempY);
	tempY = engineInstance->GetScreenSize().y - tempY;
	if (firstUpdate) {
		prevMouseX = mouseX = tempX;
		prevMouseY = mouseY = tempY;
		firstUpdate = false;
	}
	else if (tempX != mouseX || tempY != mouseY) {
		prevMouseX = mouseX;
		prevMouseY = mouseY;
		mouseX = tempX;
		mouseY = tempY;
	}
}
