//Code originating from Rastertek has been utilised and modified within this class
#include "Light.h"

Light::Light()
{

}

Light::~Light()
{

}


void Light::SetAmbientColour(float red, float green, float blue, float alpha)
{
	m_ambientColour = XMFLOAT4(red, green, blue, alpha);
	return;
}

void Light::SetDiffuseColour(float red, float green, float blue, float alpha)
{
	m_diffuseColour = XMFLOAT4(red, green, blue, alpha);
	return;
}

void Light::SetSpecularColour(float red, float green, float blue, float alpha)
{
	m_specularColour = XMFLOAT4(red, green, blue, alpha);
	return;
}

void Light::SetSpecularPower(float power)
{
	m_specularPower = power;
	return;
}


void Light::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}

XMFLOAT4 Light::GetAmbientColour()
{
	return m_ambientColour;
}

XMFLOAT4 Light::GetDiffuseColour()
{
	return m_diffuseColour;
}

XMFLOAT4 Light::GetSpecularColour()
{
	return m_specularColour;
}

float Light::GetSpecularPower()
{
	return m_specularPower;
}


XMFLOAT3 Light::GetDirection()
{
	return m_direction;
}