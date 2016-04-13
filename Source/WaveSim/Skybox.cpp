//Code originating from Rastertek has been utilised and modified within this class
#include "Skybox.h"

Skybox::Skybox()
{
	m_skyBoxAmbientLight = 0;

}



Skybox::~Skybox()
{


}

void Skybox::Initialise(ID3D11Device *device, HWND hwnd)
{
	GameObject::Initialise(device, hwnd);

	Mesh* skyMesh = new Mesh;
	ID3D11DeviceContext* context;
	device->GetImmediateContext(&context);
	skyMesh->Initialise(device, context, "skydome.txt", "../WaveSim/Data/Textures/skybox.tga");
	SetMesh(skyMesh);

	//Initialise ambient light
	m_skyBoxAmbientLight = new Light;
	m_skyBoxAmbientLight->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_skyBoxAmbientLight->SetSpecularColour(0.0f, 0.0f, 0.0f, 0.0f);
	m_skyBoxAmbientLight->SetSpecularPower(10000.0f);
	m_skyBoxAmbientLight->SetAmbientColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_skyBoxAmbientLight->SetDirection(0, -1, 1);
}

void Skybox::Render(ID3D11DeviceContext *context, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, Light *light, XMFLOAT3 cameraPos)
{
	SetPosition(cameraPos);
	GameObject::Render(context, viewMatrix, projectionMatrix, m_skyBoxAmbientLight, cameraPos);
}