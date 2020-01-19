#include "DivisionPCH.h"
#include "Engine/Core/GameSceneManager.h"
#include "Engine/Core/Debug/Debug.h"

  
int main(int argc, char* args[]) {

	std::string name = { "Division Game Engine" };
	Debug::DebugInit(name + "_Log");
	Debug::Info("Starting the GameSceneManager", __FILE__, __LINE__);
	
	GameSceneManager* gsm = new GameSceneManager();
	if (gsm->Init(name, 1280, 720) ==  true) {
		gsm->Run();
	} 
	delete gsm;
	
	_CrtDumpMemoryLeaks();
	exit(0);
}