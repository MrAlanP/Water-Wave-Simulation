#pragma once

#include "GameObject.h"
#include "Camera.h"

class Skybox : public GameObject
{
public:
	Skybox();
	~Skybox();

	virtual void Initialise(ID3D11Device*, HWND);
	virtual void Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, Light*, XMFLOAT3);


private:
	Light *m_skyBoxAmbientLight;
};