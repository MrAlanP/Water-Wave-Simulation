#pragma once

#include "Mesh.h"
#include "WaterShader.h"

class GameObject
{
public:
	GameObject();
	GameObject(const GameObject&);
	~GameObject();

	virtual void Initialise(ID3D11Device*, HWND);
	virtual void Shutdown();
	virtual void Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, Light*, XMFLOAT3);

	void SetMesh(Mesh*);
	Mesh* GetMesh();

	XMMATRIX GetModelMatrix();

	void SetPosition(XMFLOAT3);

private:
	virtual void CreateShader(ID3D11Device*, HWND);
	virtual void RenderShader(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, Light*, XMFLOAT3);

protected:
	void RenderMesh(ID3D11DeviceContext*);
	
	DirectX::XMFLOAT3	m_position;
	DirectX::XMFLOAT3	m_rotation;
	DirectX::XMFLOAT3	m_scale;
	DirectX::XMFLOAT3	m_velocity;
	DirectX::XMFLOAT4X4 m_modelMatrix;

	Mesh*			m_mesh;
	float			m_alpha;

private:
	TextureShader*	m_textureShader;


};