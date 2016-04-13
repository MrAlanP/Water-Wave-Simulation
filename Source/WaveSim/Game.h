#pragma once

#include "Gamerenderer.h"
#include "Input.h"
#include "WaveManager.h"
#include "Skybox.h"
#include "Mesh.h"

#include <vector>



class Game
{
public:
	Game();
	~Game();
	
	void Initialise(HINSTANCE, HWND, int, int);
	void Update(float dt);
	void Shutdown();

	Input* GetInput();

private:

	void CreateGameobject(GameObject*, Mesh*, XMFLOAT3);
	void CreateLight();

	vector<GameObject*> m_renderObjects;
	
	WaveManager		*m_waveManager;
	Input			*m_Input;
	GameRenderer	*m_Graphics;
	Camera			*m_Camera;
	Light			*m_light;
	HWND			m_hwnd;
	Skybox			*m_sky;
	TwBar			*m_gameTweakBar;

	bool			*m_wireframe;
};


