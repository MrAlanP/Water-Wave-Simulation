#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <vector>
#include <AntTweakBar.h>
#include "Water.h"
#include "PlaneMesh.h"

using namespace DirectX;
using namespace std;

class WaveManager
{

public:
	WaveManager();
	~WaveManager();

	void Initialise(ID3D11Device*, ID3D11DeviceContext*, TwBar*);
	void Update(float);
	void Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, Light*, XMFLOAT3, ID3D11ShaderResourceView*);
	void Shutdown();
	void CreateWater(XMFLOAT3, ID3D11Device*, ID3D11DeviceContext*, HWND);
	void AddWaveForce(TwBar*);
	void NextPreset();
	


private:
	class WavePreset
	{
	public:
		WaveForce *m_wave_forces;
	
		WavePreset()
		{
			m_wave_forces = new WaveForce[8];
		}
	};

	void SetPreset(int);


	std::vector<Water*>	m_waterObjects;

	float				m_time;
	int					m_assignedWaveForces,
						m_waveForceCount;
	ID3D11ComputeShader	*m_computeShader;
	WaveForce			*m_waveForces;


	int					m_currentPreset,
						m_presetCount;
	WavePreset			*m_wavePresets;
}; 
