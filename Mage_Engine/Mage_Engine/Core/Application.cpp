#include "Application.h"


Application::Application() : m_open(true), m_time()
{
}

void Application::Run()
{
	Initialization();
	float deltaTime;
	while (m_open)
	{
		deltaTime = m_time.TimeStep();
		OnGUI();
		OnUpdate(deltaTime);
	}
}

void Application::Initialization()
{
}

void Application::OnGUI()
{
}

void Application::OnUpdate(const float &dt)
{
}


Application::~Application()
{
}
