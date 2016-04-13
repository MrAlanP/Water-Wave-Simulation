#pragma once

#include "GameObject.h"
#include "PlaneMesh.h"

class Water : public GameObject
{
public:
	Water();
	~Water();

	void Initialise(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, float);
	virtual void Update(float);
	void Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, Light*, XMFLOAT3, ID3D11ShaderResourceView*, WaveForce*);
	virtual void Shutdown();
	
private:
	virtual void CreateShader(ID3D11Device*, HWND);
	void RenderShader(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, Light*, XMFLOAT3, ID3D11ShaderResourceView*, WaveForce*);

	WaterShader*	m_waterShader;
};