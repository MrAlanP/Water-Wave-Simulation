#pragma once
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

class WaveForce
{
public:
	WaveForce();
	WaveForce(XMFLOAT2, float, float, float);
	~WaveForce();

	void Shutdown();

	
	float	m_amplitude,
			m_speed,
			m_waveLength;
	XMFLOAT2 m_direction;

};