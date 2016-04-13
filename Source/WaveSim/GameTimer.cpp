//Code originating from Rastertek has been utilised and modified within this class
#include "GameTimer.h"
#include "Console.h"
GameTimer::GameTimer()
{

}

GameTimer::~GameTimer()
{

}

void GameTimer::Initialise()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	//How many time the frequency counter ticks per millisec
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	m_fps = 0;
	m_count = 0;
	m_fpsStartTime = timeGetTime();
}

void GameTimer::Update()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime);

	timeDifference = (float)(currentTime - m_startTime);
	m_frameTime = timeDifference / m_ticksPerMs;
	m_startTime = currentTime;


	//Update fps
	m_count++;
	if (timeGetTime() >= (m_fpsStartTime + 1000))
	{
		m_fps = m_count;
		m_count = 0;
		m_fpsStartTime = timeGetTime();
		Console::Log(m_fps);
	}

	
}

float GameTimer::GetTime()
{
	return m_frameTime*0.0016f;
}

int GameTimer::GetFps()
{
	return m_fps;
}
