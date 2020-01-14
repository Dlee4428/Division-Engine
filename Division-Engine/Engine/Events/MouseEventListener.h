#ifndef MOUSEEVENTLISTENER_H
#define MOUSEEVENTLISTENER_H

#include "../../DivisionPCH.h"

class CoreEngine;
class MouseEventListener
{
public:
	MouseEventListener(const MouseEventListener&) = delete;
	MouseEventListener(MouseEventListener&&) = delete;
	MouseEventListener& operator=(const MouseEventListener&) = delete;
	MouseEventListener& operator=(MouseEventListener&&) = delete;

	MouseEventListener() = delete;
	~MouseEventListener();

	// Register CoreEngine Pointer
	static void RegisterEngineObject(CoreEngine* engine_);
	
	// Update SDL_Event
	static void Update(SDL_Event e_);
	
	// Mouse Pressed Event
	static void NotifyMousePressed(int buttonType_);
	
	// Mouse Released Event
	static void NotifyMouseReleased(int buttonType_);
	
	// Mouse Movement Event
	static void NotifyMouseMove();
	
	// Mouse Scroll Event (Zoom and Out)
	static void NotifyMouseScroll(int y_);

	// Previous Mouse Position
	static glm::vec2 GetPreviousMousePos();
	
	// Current Mouse Position
	static glm::vec2 GetMousePos();
	
	// Mouse Offset Getters
	static glm::vec2 GetMouseOffset();
private:
	static CoreEngine* engineInstance;
	static int mouseX, mouseY, prevMouseX, prevMouseY;
	static bool firstUpdate;

	static void UpdateMousePos();
};

#endif