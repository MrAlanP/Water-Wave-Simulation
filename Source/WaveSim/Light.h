//Code originating from Rastertek has been utilised and modified within this class

#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Light
{
public:
	Light();
	~Light();

	void SetAmbientColour(float, float, float, float);
	void SetDiffuseColour(float, float, float, float);
	void SetSpecularColour(float, float, float, float);
	void SetSpecularPower(float);
	void SetDirection(float, float, float);

	XMFLOAT4 GetAmbientColour();
	XMFLOAT4 GetDiffuseColour();
	XMFLOAT3 GetDirection();
	XMFLOAT4 GetSpecularColour();
	float	 GetSpecularPower();

private:

	XMFLOAT4 m_ambientColour;
	XMFLOAT4 m_diffuseColour;
	XMFLOAT4 m_specularColour;
	float	 m_specularPower;
	XMFLOAT3 m_direction;
};