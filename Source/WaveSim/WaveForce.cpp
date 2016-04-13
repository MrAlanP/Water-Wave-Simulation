#include "WaveForce.h"

WaveForce::WaveForce()
{
	m_direction = XMFLOAT2(0,0);
	m_speed = 0;
	m_amplitude = 0;
	m_waveLength = 0;
}

WaveForce::WaveForce(XMFLOAT2 waveDirection, float speed, float amplitude, float waveLength)
{
	float directionMagnitude = sqrt((waveDirection.x*waveDirection.x) + (waveDirection.y*waveDirection.y));

	m_direction = XMFLOAT2(waveDirection.x/directionMagnitude, waveDirection.y/directionMagnitude);
	m_direction = waveDirection;
	m_speed = speed;
	m_amplitude = amplitude;
	m_waveLength = waveLength;
}

WaveForce::~WaveForce()
{

}

void WaveForce::Shutdown()
{

}

