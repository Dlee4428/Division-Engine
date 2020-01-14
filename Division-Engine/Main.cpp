#include "Engine/Core/CoreEngine.h"
#include "Engine/Game/Game1.h"

int main(int argr, char* argv[]) {
	CoreEngine::GetInstance()->SetGameInterface(new Game1);

	if (!CoreEngine::GetInstance()->OnCreate("Division Engine", 800, 600)) {
		Debug::FatalError("Engine creation failed", "Main.cpp", __LINE__);
		return 0;
	}
	CoreEngine::GetInstance()->Run();

	return 0;
}