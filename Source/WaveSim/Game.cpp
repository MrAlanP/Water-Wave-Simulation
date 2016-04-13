#include "Game.h"
#include "Console.h"
#include <iostream>
#include <AntTweakBar.h>


Game::Game()
{
	m_Input = 0;
	m_Graphics = 0;
	m_Camera = 0;
	m_waveManager = 0;
	m_sky = 0;
	m_wireframe = false;
}

Game::~Game()
{

}

//Toggle wireframe AntTweakBar callback
void TW_CALL Callback(void *clientData)
{
	//Toggle wireframe on/off 
	GameRenderer* gr = static_cast<GameRenderer*>(clientData);
	gr->GetDirectXBase()->ToggleWireframe();
}

void TW_CALL NextPreset(void *clientData)
{
	WaveManager* wm = static_cast<WaveManager*>(clientData);
	wm->NextPreset();
}

void Game::Initialise(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	//Initialise Input
	m_Input = new Input;
	m_Input->Initialise(hinstance,hwnd,screenWidth,screenHeight);

	//Initialise Graphics
	m_Graphics = new GameRenderer;
	m_Graphics->Initialise(screenWidth, screenHeight, hwnd);


	//Initialise AntTweakBar
	TwInit(TW_DIRECT3D11, m_Graphics->GetDevice());
	TwWindowSize(screenWidth, screenHeight);

	//Create the camera
	m_Camera = new Camera;
	m_Camera->SetPosition(0, 25, -75);

	m_Camera->SetRotation(10, 0, 0);

	//Create the skybox
	m_sky = new Skybox;
	m_sky->Initialise(m_Graphics->GetDevice(), hwnd);
	m_sky->SetPosition(XMFLOAT3(0, 0, 0));

	//Create the ant tweak bar
	m_gameTweakBar = TwNewBar("Water Wave Simulation");
	int barSize[2] = { 200, 680 };
	TwSetParam(m_gameTweakBar, NULL, "size", TW_PARAM_INT32, 2, barSize);

	//Toggle wireframe on/off
	TwAddButton(m_gameTweakBar, "comment1", Callback, m_Graphics, " label='Toggle Wireframe' ");


	//Create the wave manager
	m_waveManager = new WaveManager;
	TwAddButton(m_gameTweakBar, "comment2", NextPreset, m_waveManager, " label='Next Preset' ");
	m_waveManager->Initialise(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), m_gameTweakBar);
	
	m_waveManager->CreateWater(XMFLOAT3(-480, 0, -480), m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), hwnd);

	m_hwnd = hwnd;

	//Create terrain 
	Mesh* terrainMesh = new Mesh;
	terrainMesh->Initialise(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), "terrain.txt", "../WaveSim/Data/Textures/sand.tga");
	//Use same mesh for multiple instances
	CreateGameobject(new GameObject, terrainMesh, XMFLOAT3(1500, -10, 2000));
	CreateGameobject(new GameObject, terrainMesh, XMFLOAT3(10, -10, 1000));
	CreateGameobject(new GameObject, terrainMesh, XMFLOAT3(-1000, -10, 0));
	Console::Log("Terrain Initalised...");
	//Create directional light
	CreateLight();
}

void Game::Update(float dt)
{
	m_Input->Update();
	//If escape is pressed, exit program
	if (m_Input->IsKeyDown(DIK_ESCAPE))
	{
		exit(0);
	}

	m_Camera->Update(dt, m_Input);
	m_waveManager->Update(dt);

	//Refresh anttweakbar
	TwRefreshBar(m_gameTweakBar);


	//Render scene
	m_Graphics->Render(m_Camera, m_renderObjects, m_sky, m_waveManager, m_light);
}

void Game::Shutdown()
{
	//Terminate AntTweakBar
	TwTerminate();

	//Release the graphics object
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	//Release WaveManager
	if (m_waveManager)
	{
		m_waveManager->Shutdown();
		delete m_waveManager;
		m_waveManager = 0;
	}

	//Release the camera object
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}


	//Release light
	delete m_light;
	m_light = 0;


	//Release all gameobjects
	for (vector<GameObject*>::iterator it = m_renderObjects.begin(); it != m_renderObjects.end(); ++it)
	{
		(*it)->Shutdown();
		delete *it;
		*it = 0;
	}

	m_renderObjects.clear();
	
	//Release the input object
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}
}

void Game::CreateGameobject(GameObject* gameobject, Mesh* Mesh, XMFLOAT3 position)
{
	gameobject->Initialise(m_Graphics->GetDirectXBase()->GetDevice(), m_hwnd);
	gameobject->SetPosition(position);
	gameobject->SetMesh(Mesh);
	//Add to list of obejcts to render
	m_renderObjects.push_back(gameobject);
}

void Game::CreateLight()
{
	//Create the light object
	m_light = new Light;
	m_light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetSpecularColour(0.1f, 0.1f, 0.1f, 0.1f);
	m_light->SetSpecularPower(500.0f);
	m_light->SetAmbientColour(0.4f, 0.4f, 0.4f, 1.0f);
	m_light->SetDirection(0, -0.1f, -1);
}

Input* Game::GetInput()
{
	return m_Input;
}