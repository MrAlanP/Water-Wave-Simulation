//Code originating from Rastertek has been utilised and modified within this class
#pragma once
#pragma comment(lib, "winmm.lib")


#include <Windows.h>
#include <mmsystem.h>

class GameTimer
{
public:
	GameTimer();
	~GameTimer();

	void Initialise();
	void Update();

	float GetTime();
	int GetFps();

private:
	INT64			m_frequency;
	float			m_ticksPerMs;
	INT64			m_startTime;
	float			m_frameTime;
	int				m_fps;
	int				m_count;
	unsigned long	m_fpsStartTime;

};