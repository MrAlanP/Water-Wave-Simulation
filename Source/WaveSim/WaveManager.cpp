#include "WaveManager.h"

WaveManager::WaveManager()
{
	m_time = 0;
	m_assignedWaveForces = 0;
	m_waveForceCount = 8;
	m_waveForces = new WaveForce[m_waveForceCount];
	m_presetCount = 3;
	m_wavePresets = new WavePreset[m_presetCount];
	m_currentPreset = 0;
}

WaveManager::~WaveManager()
{

}


void WaveManager::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, TwBar* tweakBar)
{
	//Add waves
	for (int i = 0; i < m_waveForceCount; i++)
	{
		AddWaveForce(tweakBar);
	}

	//Creating wave presets
	m_wavePresets[0] = *(new WavePreset());
	m_wavePresets[0].m_wave_forces[0] = *(new WaveForce(XMFLOAT2(0.8f, -0.2f), 8.5f, 1.6f, 90.0f));
	m_wavePresets[0].m_wave_forces[1] = *(new WaveForce(XMFLOAT2(-0.4f, -0.1f), 7.0f, 0.6f, 56.0f));
	m_wavePresets[0].m_wave_forces[2] = *(new WaveForce(XMFLOAT2(-0.6f, -0.3f), 1.0f, 0.2f, 24.0f));
	m_wavePresets[0].m_wave_forces[3] = *(new WaveForce(XMFLOAT2(0.4f, -0.3f), 2.0f, 0.6f, 32.0f));
	m_wavePresets[0].m_wave_forces[4] = *(new WaveForce(XMFLOAT2(0.3f, -0.2f), 1.0f, 0.8f, 42.0f));
	m_wavePresets[0].m_wave_forces[5] = *(new WaveForce(XMFLOAT2(-0.5f, 0.3f), 1.0f, 0.6f, 36.0f));
	m_wavePresets[0].m_wave_forces[6] = *(new WaveForce(XMFLOAT2(0.6f, -0.3f), 2.0f, 0.7f, 40.0f));
	m_wavePresets[0].m_wave_forces[7] = *(new WaveForce(XMFLOAT2(0.6f, -0.3f), 1.0f, 0.5f, 46.0f));

	m_wavePresets[1] = *(new WavePreset());
	m_wavePresets[1].m_wave_forces[0] = *(new WaveForce(XMFLOAT2(0.1f, 0.8f), 15.0f, 0.7f, 90));
	m_wavePresets[1].m_wave_forces[1] = *(new WaveForce(XMFLOAT2(0.5f, -0.1f), 8.3f, 0.3f, 94));
	m_wavePresets[1].m_wave_forces[2] = *(new WaveForce(XMFLOAT2(-0.8f, 0.4f), 2.6f, 0.6f, 62));
	m_wavePresets[1].m_wave_forces[3] = *(new WaveForce(XMFLOAT2(-0.8f, -0.9f), 3.7f, 0.7f, 67));
	m_wavePresets[1].m_wave_forces[4] = *(new WaveForce(XMFLOAT2(0.9f, -0.7f), 1.0f, 0.3f, 71));
	m_wavePresets[1].m_wave_forces[5] = *(new WaveForce(XMFLOAT2(1.0f, -0.7f), 2.3f, 0.1f, 60));
	m_wavePresets[1].m_wave_forces[6] = *(new WaveForce(XMFLOAT2(0.2f, 0.6f), 3.4f, 0.2f, 40));
	m_wavePresets[1].m_wave_forces[7] = *(new WaveForce(XMFLOAT2(0.8f, -0.3f), 1.2f, 0.5f, 60));

	m_wavePresets[2] = *(new WavePreset());
	m_wavePresets[2].m_wave_forces[0] = *(new WaveForce(XMFLOAT2(0.1f, -0.4f), 15.0f, 0.6f, 55));
	m_wavePresets[2].m_wave_forces[1] = *(new WaveForce(XMFLOAT2(-1.0f, -1.0f), 27.5f, 0.5f, 100));
	m_wavePresets[2].m_wave_forces[2] = *(new WaveForce(XMFLOAT2(0.7f, 0), 1, 0.5f, 24));
	m_wavePresets[2].m_wave_forces[3] = *(new WaveForce(XMFLOAT2(1, 0), 3.7f, 0.4f, 32));
	m_wavePresets[2].m_wave_forces[4] = *(new WaveForce(XMFLOAT2(-0.1f,-0.2f), 1, 2.1f, 51));
	m_wavePresets[2].m_wave_forces[5] = *(new WaveForce(XMFLOAT2(0.5f, 0.7f), 1, 0.6f, 100));
	m_wavePresets[2].m_wave_forces[6] = *(new WaveForce(XMFLOAT2(-0.3f, -0.3f), 2, 0.8f, 40));
	m_wavePresets[2].m_wave_forces[7] = *(new WaveForce(XMFLOAT2(0.1f, -1.0f), 1, 0.6f, 100));

	//Set waves to first preset
	SetPreset(0);
}

void WaveManager::Update(float dt)
{
	for (std::vector<Water*>::iterator it = m_waterObjects.begin(); it != m_waterObjects.end(); ++it) {
		(*it)->Update(dt);
	} 
}

void WaveManager::Render(ID3D11DeviceContext *context, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, Light *light, XMFLOAT3 cameraPos, ID3D11ShaderResourceView* reflectionTexture)
{
	for (std::vector<Water*>::iterator it = m_waterObjects.begin(); it != m_waterObjects.end(); ++it) {
		(*it)->Render(context, viewMatrix, projectionMatrix, light, cameraPos, reflectionTexture, m_waveForces);
	}
}

void WaveManager::Shutdown()
{
	for (std::vector<Water*>::iterator it = m_waterObjects.begin(); it != m_waterObjects.end(); ++it) {
		(*it)->Shutdown();
	}
}

void WaveManager::CreateWater(XMFLOAT3 position, ID3D11Device *device, ID3D11DeviceContext *context, HWND hwnd)
{
	Water* water = new Water();
	water->Initialise(device, context, hwnd, 120, 120, 8.0f);
	Console::Log("Main Water Initialised...");
	water->SetPosition(position);
	m_waterObjects.push_back(water);

	//Create lower resolution water meshes surrounding initial plane
	for (int i = 0; i < 4; i++)
	{

		Water* waterB = new Water();
		m_waterObjects.push_back(waterB);


		XMFLOAT2 dimensions = XMFLOAT2(180, 120);
		XMFLOAT2 offset = XMFLOAT2(0, 959);
		if (i==1)
		{
			offset = XMFLOAT2(959, -1919);
			dimensions = XMFLOAT2(dimensions.y, dimensions.x);
		}
		else if (i == 2)
		{
			offset = XMFLOAT2(-1919, -1919);
		}
		else if (i == 3)
		{
			offset = XMFLOAT2(-1919, 0);
			dimensions = XMFLOAT2(dimensions.y, dimensions.x);
		}

		waterB->Initialise(device, context, hwnd, dimensions.x, dimensions.y, 16.0f);
		string initText = "Mid Water Initialised...";
		Console::Log(initText);
		waterB->SetPosition(XMFLOAT3(position.x + offset.x, position.y-1, position.z + offset.y));
	}

	//Even further out 
	for (int i = 0; i < 4; i++)
	{
		Water* waterB = new Water();
		m_waterObjects.push_back(waterB);


		XMFLOAT2 dimensions = XMFLOAT2(270, 120);
		XMFLOAT2 offset = XMFLOAT2(-1918, 2878);
		if (i == 1)
		{
			offset = XMFLOAT2(2878, -5758);
			dimensions = XMFLOAT2(dimensions.y, dimensions.x);
		}
		else if (i == 2)
		{
			offset = XMFLOAT2(-5758, -5758);
		}
		else if (i == 3)
		{
			offset = XMFLOAT2(-5758, -1918);
			dimensions = XMFLOAT2(dimensions.y, dimensions.x);
		}

		waterB->Initialise(device, context, hwnd, dimensions.x, dimensions.y, 32.0f);
		string initText = "Far Water Initialised...";
		Console::Log(initText);
		waterB->SetPosition(XMFLOAT3(position.x + offset.x, position.y-2, position.z + offset.y));
	}

	
}

void WaveManager::AddWaveForce(TwBar* tweakBar)
{
	WaveForce* waveForce = new WaveForce();
	
	m_waveForces[m_assignedWaveForces] = *waveForce;


	string waveNum = "Wave " + to_string(m_assignedWaveForces+1);

	//Speed variable
	string behaviour = " min=0 max=30 step=0.1 group='"+ waveNum +"'  label='Speed' ";
	TwAddVarRW(tweakBar, "", TW_TYPE_FLOAT, &m_waveForces[m_assignedWaveForces].m_speed, &behaviour[0u]);
	//Amplitude
	behaviour = " min=0 max=10 step=0.1 group='" + waveNum + "'  label='Amplitude'";
	TwAddVarRW(tweakBar, "", TW_TYPE_FLOAT, &m_waveForces[m_assignedWaveForces].m_amplitude, &behaviour[0u]);
	//Wavelength
	behaviour = " min=20 max=100 step=1 group='" + waveNum + "'  label='Wavelength' ";
	TwAddVarRW(tweakBar, "", TW_TYPE_FLOAT, &m_waveForces[m_assignedWaveForces].m_waveLength, &behaviour[0u]);

	//Direction
	behaviour = " min=-1 max=1 step=0.1 group='" + waveNum + "'  label='Direction.x' ";
	TwAddVarRW(tweakBar, "", TW_TYPE_FLOAT, &m_waveForces[m_assignedWaveForces].m_direction.x, &behaviour[0u]);
	behaviour = " min=-1 max=1 step=0.1 group='" + waveNum + "'  label='Direction.y'";
	TwAddVarRW(tweakBar, "", TW_TYPE_FLOAT, &m_waveForces[m_assignedWaveForces].m_direction.y, &behaviour[0u]);


	//Increase wave count
	m_assignedWaveForces++;

}

void WaveManager::NextPreset()
{
	int preset = m_currentPreset+1;
	if (preset >= m_presetCount)
	{
		preset = 0;
	}

	SetPreset(preset);
	
}

void WaveManager::SetPreset(int preset)
{
	m_currentPreset = preset;

	for (int i = 0; i < m_waveForceCount; i++)
	{
		m_waveForces[i] = m_wavePresets[m_currentPreset].m_wave_forces[i];
	}
}