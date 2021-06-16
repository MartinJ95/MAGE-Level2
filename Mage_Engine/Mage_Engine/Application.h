#pragma once
#include "TimeManager.h"

class Application
{
public:
	Application();
	void Run();
	virtual void Initialization();
	virtual void OnGUI();
	virtual void OnUpdate(float dt);
	virtual ~Application();
	TimeManager m_time;
};

