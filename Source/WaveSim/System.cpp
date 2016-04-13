//Code originating from Rastertek has been utilised and modified within this class
#include "System.h"


System::System()
{
	
}



System::~System()
{
}


void System::Initialise()
{
	int screenWidth, screenHeight;

	screenWidth = 0;
	screenHeight = 0;

	//Initialise the windows api
	InitialiseWindows(screenWidth, screenHeight);

	//Initialise the game timer
	m_gameTimer = new GameTimer;
	m_gameTimer->Initialise();

	

	//Initialise the game
	m_game = new Game;
	
	m_game->Initialise(m_hinstance,m_hwnd,screenWidth, screenHeight);
}


void System::Shutdown()
{
	//Shutdown the game
	if (m_game)
	{
		m_game->Shutdown();
		delete m_game;
		m_game = 0;
	}

	// Shutdown the window.
	ShutdownWindows();
	
	return;
}


void System::Run()
{
	MSG msg;
	bool done;


	//Initialise the message structure
	ZeroMemory(&msg, sizeof(MSG));
	
	//Loop until there is a quit message from the window or the user
	done = false;
	while(!done)
	{
		//Handle the windows messages
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//If windows signals to end the application then exit out
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//Update the System
			Update();
		}

	}

	return;
}


void System::Update()
{
	//Update game timer
	m_gameTimer->Update();
	float dt = m_gameTimer->GetTime();


	//Update game
	m_game->Update(dt);
	

}


LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	if (TwEventWin(hwnd, umsg, wparam, lparam)) //send event message to AntTweakBar
		return 0; //event has been handled by AntTweakBar
	//else process the event message
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}


void System::InitialiseWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	//Get an external pointer to this object
	ApplicationHandle = this;

	//Get the instance of this application
	m_hinstance = GetModuleHandle(NULL);

	//Give the application a name
	m_applicationName = L"Wave Simulation";

	//Setup the windows class with default settings
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	//Register the window class
	RegisterClassEx(&wc);

	//Determine the resolution of the clients desktop screen
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//Setup the screen settings depending on whether it is running in full screen or in windowed mode
	if(FULL_SCREEN)
	{
		//If full screen set the screen to maximum size of the users desktop and 32bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner
		posX = posY = 0;
	}
	else
	{
		screenWidth = 1920;
		screenHeight = 1080;

		//Place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	//Create the window with the screen settings and get the handle to it
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						    posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	//Bring the window up on the screen and set it as main focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);


	return;
}


void System::ShutdownWindows()
{
	//Show the mouse cursor
	ShowCursor(true);

	//Fix the display settings if leaving full screen mode
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	//Remove the window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//Remove the application instance
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	//Release the pointer to this class
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		//Check if the window is being destroyed
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		//Check if the window is being closed
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		//All other messages pass to the message handler in the system class
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}