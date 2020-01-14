#ifndef WINDOW_H
#define WINDOW_H

#include "../../DivisionPCH.h"
#include "../Core/Debug/Debug.h"

class Window
{
public:
	Window();
	~Window();

	// Init Window
	bool OnCreate(std::string name_, int width_, int height_);

	// Destroy Window
	void OnDestroy();

	// Window Getters
	int GetWidth() const;

	// Window Setters
	int GetHeight() const;

	// Reference Window pointer getters 
	SDL_Window* GetWindow() const;

private:
	// Window Attributes
	void SetAttributes();
	int width;
	int height;
	SDL_Window* window;
	SDL_GLContext context;
	std::string glsl_version_string = "#version 450";
};
#endif // !WINDOW_H


