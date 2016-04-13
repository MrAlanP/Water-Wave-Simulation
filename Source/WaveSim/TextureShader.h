//Code originating from Rastertek has been utilised and modified within this class

#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include "Light.h"
using namespace DirectX;
using namespace std;

__declspec(align(16)) class TextureShader
{
private:
	//Buffers
	struct MatrixBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct CameraBuffer
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	struct LightBuffer
	{
		XMFLOAT4 ambientColour;
		XMFLOAT4 diffuseColour;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColour;
	};
	struct AlphaBuffer
	{
		float alphaAmount;
		XMFLOAT3 padding;
	};
public:
	TextureShader();
	~TextureShader();

	virtual void Initialise(ID3D11Device*, HWND);
	virtual void Render(ID3D11DeviceContext*, int, XMMATRIX*, XMMATRIX*, XMMATRIX*, ID3D11ShaderResourceView*, Light*, XMFLOAT3, float);
	virtual void Update(float) {};
	void Shutdown();

protected:
	virtual void InitialiseShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	virtual void ShutdownShader();
	virtual void SetVertexShaderParams(ID3D11DeviceContext*, XMMATRIX*, XMMATRIX*, XMMATRIX*, XMFLOAT3);
	virtual void SetPixelShaderParams(ID3D11DeviceContext*, ID3D11ShaderResourceView*, Light*, float);
	virtual void RenderShader(ID3D11DeviceContext*, int);
	void SetShaderParams(ID3D11DeviceContext*, XMMATRIX*, XMMATRIX*, XMMATRIX*, ID3D11ShaderResourceView*, Light*, XMFLOAT3, float);
	void SetShaderMatrixBuffer(ID3D11DeviceContext*, XMMATRIX*, XMMATRIX*, XMMATRIX*);

	void CreateBuffer(ID3D11Device*, ID3D11Buffer**, int);
	
private:
	
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	ID3D11VertexShader	*m_vertexShader;
	ID3D11PixelShader	*m_pixelShader;
	ID3D11InputLayout	*m_layout;
	ID3D11SamplerState	*m_sampleState;
	ID3D11Buffer		*m_matrixBuffer;
	ID3D11Buffer		*m_lightBuffer;
	ID3D11Buffer		*m_cameraBuffer;
	ID3D11Buffer		*m_alphaBuffer;

};