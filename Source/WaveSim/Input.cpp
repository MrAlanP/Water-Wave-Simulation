//Code originating from Rastertek has been utilised and modified within this class
#include "Input.h"


Input::Input()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;

	//Initialise mouse position
	m_mouseX = 0;
	m_mouseY = 0;
	m_mouseL = XMFLOAT2(0, 0);
}

Input::~Input()
{
}

void Input::Initialise(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	//Set the screen dimensions
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//Initialise direct input
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		Console::Log("Error: Failed to initialise Direct Input.");
	}

	//Initialise the keyboard
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		Console::Log("Error: Failed to initialise Keyboard.");
	}

	//Set the data format.  In this case since it is a keyboard we can use the predefined data format
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		Console::Log("Error: Failed to Set Keyboard Data.");
	}

	//Set the cooperative level of the keyboard to not share with other programs
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		Console::Log("Error: Failed to Set Keyboard cooperative level.");
	}

	//Initialise the mouse
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		Console::Log("Error: Failed to initialise Mouse.");
	}

	//Set the data format for the mouse using the pre-defined mouse data format
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		Console::Log("Error: Failed to Set Mouse Data.");
	}

	//Set the cooperative level of the mouse to share with other programs
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		Console::Log("Error: Failed to Set Mouse cooperative level.");
	}

	Update();
}

void Input::Shutdown()
{
	//Release the mouse
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	//Release the keyboard
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	//Release direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

void Input::Update()
{
	//Read inputs
	ReadKeyboard();
	ReadMouse();

	ProcessInput();
}

void Input::ReadKeyboard()
{
	HRESULT result;

	//Read the keyboard
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		//If the keyboard lost focus or was not acquired then try to get control back
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			Console::Log("Error: Could not read keyboard.");
		}
	}
}


void Input::ReadMouse()
{
	HRESULT result;

	//Read the mouse
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		//If the mouse lost focus or was not acquired then try to get control back
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			Console::Log("Error: Could not read mouse.");
		}
	}
}

void Input::ProcessInput()
{
	//Update the position of the mouse
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	m_mouseL = XMFLOAT2(m_mouseState.lX, m_mouseState.lY);


	//Limit mouse position to screen
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return;
}

//Gets the mouses movement this frame
XMFLOAT2 Input::GetMouseMovement()
{
	return m_mouseL;
}

bool Input::IsKeyDown(UCHAR key)
{

	if (m_keyboardState[key] & 0x80)
	{
		m_keyboardArray[key] = 1;
		return true;
	}
	else
	{
		m_keyboardArray[key] = 0;
		return false;
	}

}

bool Input::IsKeyPressed(UCHAR key)
{
	if (m_keyboardState[key] & 0x80)
	{
		if (m_keyboardArray[key] == 1)    //key is already down
		{
			return false;
		}
		else
		{
			m_keyboardArray[key] = 1;
			return true;        //Key is now down
		}
	}
	else
	{
		m_keyboardArray[key] = 0;         //Key is obviously up so reset it
		return false;
	}
}

bool Input::IsMouseDown(int button)
{
	return m_mouseState.rgbButtons[button];
}

float Input::GetMouseScroll()
{
	return m_mouseState.lZ;
}