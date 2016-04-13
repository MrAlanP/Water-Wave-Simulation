#include "WaterShader.h"

WaterShader::WaterShader() : TextureShader()
{
	m_time = 0;
	
}

void WaterShader::Initialise(ID3D11Device* device, HWND hwnd)
{
	InitialiseShader(device, hwnd, L"../WaveSim/water_gerstner.vs", L"../WaveSim/water_gerstner.ps");

	m_foamTexture = new Texture;
	m_normalMap = new Texture;
	m_normalMap2 = new Texture;
	ID3D11DeviceContext* context;
	device->GetImmediateContext(&context);

	m_foamTexture->Initialise(device, context, "../WaveSim/Data/Textures/wave_foam.tga");
	m_normalMap->Initialise(device, context, "../WaveSim/Data/Textures/water_normal.tga");
	m_normalMap2->Initialise(device, context, "../WaveSim/Data/Textures/water_normal2.tga");

	return;
}

void WaterShader::InitialiseShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	TextureShader::InitialiseShader(device, hwnd, vsFilename, psFilename);
	
	CreateBuffer(device,&m_waveBuffer, sizeof(WaveBuffer));
	CreateBuffer(device, &m_updateBuffer, sizeof(UpdateBuffer));

	//Lock the wave constant buffer so it can be written to
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ID3D11DeviceContext* context;
	device->GetImmediateContext(&context);

	//Setup wave buffer
	context->Map(m_waveBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// Unlock the constant buffer.
	context->Unmap(m_waveBuffer, 0);
	//Get a pointer to the data in the constant buffer
	m_waveData = (WaveBuffer*)mappedResource.pData;
}

void WaterShader::ShutdownShader()
{
	TextureShader::ShutdownShader();
	// Release the light constant buffer.
	if (m_waveBuffer)
	{
		m_waveBuffer->Release();
		m_waveBuffer = 0;
	}

	return;
}

void WaterShader::Update(float dt)
{
	m_time += dt;
	return;
}


void WaterShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projMatrix,
	ID3D11ShaderResourceView* texture, Light* light, XMFLOAT3 cameraPosition, WaveForce* waveForces)
{
	SetShaderParams(deviceContext, worldMatrix, viewMatrix, projMatrix, texture, light, cameraPosition, 1.0f);

	//Copy the wave variables into the struct
	for (int i = 0; i < 8; i++){
		m_waveData->amplitude[i] = XMFLOAT4(waveForces[i].m_amplitude, 0, 0, 0);
		m_waveData->wavelength[i] = XMFLOAT4(waveForces[i].m_waveLength, 0, 0, 0);
		m_waveData->speed[i] = XMFLOAT4(waveForces[i].m_speed, 0, 0, 0);
		m_waveData->directionX[i] = XMFLOAT4(waveForces[i].m_direction.x, 0,0,0);
		m_waveData->directionY[i] = XMFLOAT4(waveForces[i].m_direction.y, 0, 0, 0);
	}
	
	

	RenderShader(deviceContext, indexCount);
	return;
}


void WaterShader::SetVertexShaderParams(ID3D11DeviceContext* deviceContext, XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix, XMFLOAT3 cameraPosition)
{
	SetShaderMatrixBuffer(deviceContext, worldMatrix, viewMatrix, projectionMatrix);

	//Update the Update CBuffer with Camera pos and time
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	UpdateBuffer* updatePtr;
	//Lock camera buffer so it can be written to
	deviceContext->Map(m_updateBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	updatePtr = (UpdateBuffer*)mappedResource.pData;
	updatePtr->cameraPosition = cameraPosition;
	updatePtr->time = m_time;
	

	//Unlock camera buffer
	deviceContext->Unmap(m_updateBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &m_updateBuffer);

	///Update the wave information in the cbuffer
	deviceContext->VSSetConstantBuffers(2, 1, &m_waveBuffer);
}

void WaterShader::SetPixelShaderParams(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, Light* light, float alpha)
{
	TextureShader::SetPixelShaderParams(deviceContext, texture, light, alpha);

	// Set shader texture resource in the pixel shader.
	ID3D11ShaderResourceView* foamTex = m_foamTexture->GetTexture();
	deviceContext->PSSetShaderResources(1, 1, &foamTex);
	ID3D11ShaderResourceView* normalTex = m_normalMap->GetTexture();
	deviceContext->PSSetShaderResources(2, 1, &normalTex);
	ID3D11ShaderResourceView* normalTex2 = m_normalMap2->GetTexture();
	deviceContext->PSSetShaderResources(3, 1, &normalTex2);
}
