#pragma once
#include <Windows.h>
/// <summary>
/// TimeManager
/// GetDeltaTime() ÇÏ¸é dt ³ª¿È
/// </summary>

class TimeManager {
public:
    TimeManager();

public:
    float GetDeltaTime() const;
    float GetFrameRate() const;

public:
    void Update();

private:
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_startCounter;
    LARGE_INTEGER m_endCounter;
    float m_deltaTime;
};
