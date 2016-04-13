#pragma once

#include "WaveForce.h"
#include "Texture.h"
#include "TextureShader.h"

using namespace DirectX;
using namespace std;

__declspec(align(16)) class WaterShader : public TextureShader
{

private:

	struct UpdateBuffer
	{
		XMFLOAT3 cameraPosition;
		float time;
	};

	struct WaveBuffer
	{
		XMFLOAT4 amplitude[8];
		XMFLOAT4 wavelength[8];
		XMFLOAT4 speed[8];
		XMFLOAT4 directionX[8];
		XMFLOAT4 directionY[8];
	};

	struct GerstnerPositions
	{
		XMFLOAT3 x;
		XMFLOAT3 y;
	};


public:
	WaterShader();

	virtual void Initialise(ID3D11Device*, HWND);
	virtual void Update(float);
	virtual void Render(ID3D11DeviceContext*, int, XMMATRIX*, XMMATRIX*, XMMATRIX*, ID3D11ShaderResourceView*, Light*, XMFLOAT3, WaveForce*);


private:
	virtual void InitialiseShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	virtual void ShutdownShader();
	virtual void SetVertexShaderParams(ID3D11DeviceContext*, XMMATRIX*, XMMATRIX*, XMMATRIX*, XMFLOAT3);
	virtual void SetPixelShaderParams(ID3D11DeviceContext*, ID3D11ShaderResourceView*, Light*, float);

	WaveBuffer		*m_waveData;
	ID3D11Buffer	*m_waveBuffer;
	ID3D11Buffer	*m_updateBuffer;
	Texture			*m_foamTexture;
	Texture			*m_normalMap;
	Texture			*m_normalMap2;
	float			m_time;
};