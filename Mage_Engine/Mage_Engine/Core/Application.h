#pragma once
#include "TimeManager.h"

class Application
{
public:
	Application();
	void Run();
	virtual void Initialization();
	virtual void OnGUI(const float &dt);
	virtual void OnUpdate(const float &dt);
	virtual ~Application();
	bool m_open;
	TimeManager m_time;
};

