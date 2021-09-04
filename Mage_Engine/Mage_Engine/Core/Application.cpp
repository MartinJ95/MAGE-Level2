#include "pch.h"
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
		OnGUI(deltaTime);
		OnUpdate(deltaTime);
	}
}

void Application::Initialization()
{
}

void Application::OnGUI(const float &dt)
{
}

void Application::OnUpdate(const float &dt)
{
}


Application::~Application()
{
}
