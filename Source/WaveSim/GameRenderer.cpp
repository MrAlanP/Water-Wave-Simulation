//Code originating from Rastertek has been utilised and modified within this class
#include "GameRenderer.h"


GameRenderer::GameRenderer()
{
	m_Direct3D = 0;
	m_textureShader = 0;
}

GameRenderer::~GameRenderer()
{
}


void GameRenderer::Initialise(int screenWidth, int screenHeight, HWND hwnd)
{

	// Create the Direct3D object.
	m_Direct3D = new DirectXBase;

	// Initialize the Direct3D object.
	bool result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
	}

	//Create the render texture
	m_renderTexture = new RenderTexture;
	m_renderTexture->Initialise(m_Direct3D->GetDevice(), screenWidth, screenHeight);

	
	return;
}


void GameRenderer::Shutdown()
{
	// Release the color shader object.
	if (m_textureShader)
	{
		m_textureShader->Shutdown();
		delete m_textureShader;
		m_textureShader = 0;
	}

	// Release the D3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}

DirectXBase* GameRenderer::GetDirectXBase()
{
	return m_Direct3D;
}

ID3D11Device* GameRenderer::GetDevice()
{
	return m_Direct3D->GetDevice();
}

ID3D11DeviceContext* GameRenderer::GetDeviceContext()
{
	return m_Direct3D->GetDeviceContext();
}


void GameRenderer::Render(Camera* camera, vector<GameObject*> gameobjects, Skybox* skybox, WaveManager* waveManager, Light* light)
{
	

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	
	RenderScene(camera,gameobjects,skybox,light);

	//RenderToTexture(camera, gameobjects, skybox, light);

	//Render the waveManager
	XMMATRIX viewMatrix, projectionMatrix;
	camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	waveManager->Render(m_Direct3D->GetDeviceContext(), viewMatrix, projectionMatrix, light, camera->GetPosition(), m_renderTexture->GetShaderResourceView());
	
	//Draw AntTweakBars
	TwDraw();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	
	return;


}

void GameRenderer::RenderScene(Camera* camera, vector<GameObject*> gameobjects, Skybox* skybox, Light* light)
{
	XMMATRIX viewMatrix, projectionMatrix;

	// Generate the view matrix based on the camera's position.
	camera->Render();

	camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);


	RenderSkybox(m_Direct3D->GetDeviceContext(), camera, skybox);
	

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	for (vector<GameObject*>::iterator it = gameobjects.begin(); it != gameobjects.end(); ++it)
	{
		//m_Direct3D->GetWorldMatrix((*it)->GetModelMatrix());
		//Render the gameobject
		(*it)->Render(m_Direct3D->GetDeviceContext(), viewMatrix, projectionMatrix, light, camera->GetPosition());
	}

	
}


void GameRenderer::RenderToTexture(Camera* camera, vector<GameObject*> gameobjects, Skybox* skybox, Light* light)
{
	m_renderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	m_renderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	//Render the scene onto the texture
	RenderScene(camera, gameobjects, skybox, light);


	//Reset render target to the back buffer again
	m_Direct3D->SetBackBufferRenderTarget();


}

void GameRenderer::RenderSkybox(ID3D11DeviceContext* context, Camera* camera, Skybox* skybox)
{

	XMMATRIX viewMatrix, projectionMatrix;
	camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	//Render the skybox
	m_Direct3D->ToggleZBuffer(false);
	m_Direct3D->ToggleCulling(false);
	skybox->Render(context, viewMatrix, projectionMatrix, NULL, camera->GetPosition());
	m_Direct3D->ToggleCulling(true);
	m_Direct3D->ToggleZBuffer(true);
}