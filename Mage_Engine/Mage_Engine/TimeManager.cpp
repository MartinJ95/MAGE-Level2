#include "pch.h"
#include "TimeManager.h"


TimeManager::TimeManager()
{
}

float TimeManager::TimeStep()
{
	m_currentTime = time(NULL);
	float timeStep = m_currentTime - m_previousTime;
	m_previousTime = m_currentTime;
	return timeStep;
}


TimeManager::~TimeManager()
{
}
