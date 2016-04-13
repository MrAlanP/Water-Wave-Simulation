#include "Water.h"

Water::Water() 
{
	m_waterShader = 0;
}

Water::~Water()
{
}

void Water::Initialise(ID3D11Device *device, ID3D11DeviceContext* context, HWND hwnd, int width, int height, float vertexOffset){
	//Initialise base Gameobject
	GameObject::Initialise(device, hwnd);

	//Intialise 
	PlaneMesh* mesh = new PlaneMesh;
	mesh->LoadModel(width, height, vertexOffset);
	mesh->Initialise(device, context, NULL, "../WaveSim/Data/Textures/water.tga");
	SetMesh(mesh);
}

void Water::CreateShader(ID3D11Device *device, HWND hwnd)
{
	m_waterShader = new WaterShader;
	// Initialise the shader
	m_waterShader->Initialise(device, hwnd);
}

void Water::Update(float dt)
{
	m_waterShader->Update(dt);
}

void Water::Render(ID3D11DeviceContext *context, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, Light *light, XMFLOAT3 cameraPos, ID3D11ShaderResourceView* reflectionTexture, WaveForce* waveForces)
{
	RenderMesh(context);
	RenderShader(context, viewMatrix, projectionMatrix, light, cameraPos, reflectionTexture, waveForces);
}

void Water::RenderShader(ID3D11DeviceContext *context, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, Light *light,  XMFLOAT3 cameraPos, ID3D11ShaderResourceView* reflectionTexture, WaveForce* waveForces)
{
	m_waterShader->Render(context, m_mesh->GetIndexCount(),
		&GetModelMatrix(), &viewMatrix, &projectionMatrix,
		//reflectionTexture, light, cameraPos, waveForces);
		m_mesh->GetTexture(), light, cameraPos, waveForces);
}

//Release water shader and gameobject
void Water::Shutdown()
{
	GameObject::Shutdown();

	if (m_waterShader)
	{
		m_waterShader->Shutdown();
		delete m_waterShader;
		m_waterShader = 0;
	}
}