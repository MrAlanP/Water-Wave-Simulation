//Code originating from Rastertek has been utilised and modified within this class
#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "Game.h"
#include "GameTimer.h"



class System
{
public:
	System();
	~System();

	void Initialise();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	void Update();
	void InitialiseWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR		m_applicationName;
	HINSTANCE	m_hinstance;
	HWND		m_hwnd;

	
	Game		*m_game;
	GameTimer	*m_gameTimer;
};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//Globals

static System *ApplicationHandle = 0;

