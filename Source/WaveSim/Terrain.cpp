#include "Terrain.h"

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
}

void Terrain::Initialise(ID3D11Device *device, HWND hwnd)
{
	GameObject::Initialise(device, hwnd);
	//Initialise and load terrain model/texture
	Mesh* terrainMesh = new Mesh;
	ID3D11DeviceContext* context;
	device->GetImmediateContext(&context);
	terrainMesh->Initialise(device, context, "terrain.txt", "../WaveSim/Data/Textures/stone.tga");
	SetMesh(terrainMesh);
}

