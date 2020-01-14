#include "Window.h"

Window::Window() : window(nullptr), context(NULL), width(0), height(0) {

}

Window::~Window() {
	OnDestroy();
}

bool Window::OnCreate(std::string name_, int width_, int height_) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		Debug::FatalError("Failed to initialize SDL", "Window.cpp", __LINE__);
		return false;
	}
	this->width = width_;
	this->height = height_;

	// c_str() converts string to char*
	window = SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

	// checker for failed to cast SDL_Window
	if (!window) {
		Debug::FatalError("Failed to create SDL_Window", "Window.cpp", __LINE__);
		return false;
	}

	context = SDL_GL_CreateContext(window);
	SetAttributes();

	GLenum err = glewInit(); // Typedef of OpenGl variable 
	if (err != GLEW_OK) {
		Debug::FatalError("Failed to initialize GLEW", "Window.cpp", __LINE__);
		return false;
	}

	glEnable(GL_DEPTH_TEST); // Render properly

	std::cout << "GPU: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	return true;
}

void Window::OnDestroy() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	window = nullptr;
}

void Window::SetAttributes() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_GL_SetSwapInterval(1);

	glewExperimental = GL_TRUE;
}

int Window::GetWidth() const {
	return width;
}

int Window::GetHeight() const {
	return height;
}

SDL_Window* Window::GetWindow() const {
	return window;
}