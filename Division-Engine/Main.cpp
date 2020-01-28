#include "Engine/Core/Debug/Debug.h"
#include "Engine/Scene/Scene.h"
  
int main() {
	Scene scene;
	if (scene.CreateGLContext(2, true)) {
		scene.OnCreate();
		scene.CreateTimerLoop();
		Debug::Info("Division Engine Running Hot!", __FILE__, __LINE__);
		return 1;
	}
	else {
		Debug::Error("Failed to Run Division Engine from start", __FILE__, __LINE__);
		printf("Failed to Run Division Engine");
		return 0;
	}

	return 0;
}