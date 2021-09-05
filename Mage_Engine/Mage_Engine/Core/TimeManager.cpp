#include "TimeManager.h"


TimeManager::TimeManager() : m_currentTime(), m_previousTime();
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
