#include "System.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	//Debug console
	AllocConsole();
	
	// Create the system object.
	System* system = new System;
	if(!system)
	{
		return 0;
	}

	// Initialize and run the system object.
	system->Initialise();

	system->Run();
	

	// Shutdown and release the system object.
	system->Shutdown();
	delete system;
	system = 0;

	return 0;
}
