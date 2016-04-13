//Code originating from Rastertek has been utilised and modified within this class

#pragma once

#include "directxbase.h"
#include "Camera.h"
#include "Light.h"
#include "WaveManager.h"
#include "Skybox.h"
#include "RenderTexture.h"
#include <vector>
#include <AntTweakBar.h>


// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 10000.0f;
const float SCREEN_NEAR = 0.1f;


class GameRenderer
{
public:
	GameRenderer();
	~GameRenderer();

	void Initialise(int, int, HWND);
	void Shutdown();
	void Render(Camera*, vector<GameObject*>, Skybox*, WaveManager*, Light*);
	DirectXBase* GetDirectXBase();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

private:
	void RenderToTexture(Camera*, vector<GameObject*>, Skybox*, Light*);
	void RenderScene(Camera*, vector<GameObject*>, Skybox*, Light*);
	void RenderSkybox(ID3D11DeviceContext*, Camera*, Skybox*);

	RenderTexture	*m_renderTexture;
	DirectXBase		*m_Direct3D;
	TextureShader	*m_textureShader;

};
