#pragma once
#include <time.h>

class TimeManager
{
public:
	TimeManager();
	float TimeStep();
	~TimeManager();
	time_t m_currentTime;
	time_t m_previousTime;
};

