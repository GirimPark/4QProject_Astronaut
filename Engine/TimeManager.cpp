#include "pch.h"
#include "TimeManager.h"

TimeManager::TimeManager()
{
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_startCounter);
}

void TimeManager::Update()
{
	QueryPerformanceCounter(&m_endCounter);
	m_deltaTime = static_cast<float>(m_endCounter.QuadPart - m_startCounter.QuadPart) / m_frequency.QuadPart;
	m_startCounter = m_endCounter;

#ifdef _DEBUG
	if (m_deltaTime > (1.f / 60.f))
		m_deltaTime = (1.f / 60.f);
#endif
}

float TimeManager::GetDeltaTime() const
{
	return m_deltaTime;
}

float TimeManager::GetFrameRate() const
{
    static int frameCnt = -1;
    static float timeElapsed = 0.f;
    static float fps = 0;

    frameCnt++;
    if (frameCnt == 0)
        return 0;

    timeElapsed += m_deltaTime;

    if (timeElapsed >= 1.0f)
    {
        fps = static_cast<float>(frameCnt);
        frameCnt = 0;
        timeElapsed -= 1.0f;
    }
    return fps;
}