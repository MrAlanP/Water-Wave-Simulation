//Code originating from Rastertek has been utilised and modified within this class
#pragma once
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#define DIRECTINPUT_VERSION 0x0800
#include <directxmath.h>
#include <dinput.h>
#include "Console.h"

using namespace DirectX;

class Input
{
public:
	Input();
	~Input();

	void Initialise(HINSTANCE, HWND, int, int);
	void Shutdown();
	void Update();
	
	XMFLOAT2 GetMouseMovement();
	bool IsKeyDown(UCHAR key);
	bool IsKeyPressed(UCHAR key);

	bool IsMouseDown(int button);
	float GetMouseScroll();

private:
	void ReadKeyboard();
	void ReadMouse();
	void ProcessInput();

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	unsigned char m_keyboardArray[256];
	DIMOUSESTATE m_mouseState;

	int			m_screenWidth, 
				m_screenHeight,
				m_mouseX, 
				m_mouseY;
	XMFLOAT2	m_mouseL;

};
