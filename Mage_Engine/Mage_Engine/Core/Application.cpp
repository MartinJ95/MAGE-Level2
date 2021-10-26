#include "Application.h"


Application::Application() : m_open(true), m_time()
{
}

void Application::Run()
{
	Initialization();
	while (m_open)
	{
		m_deltaTime = m_time.TimeStep();
		OnGUI();
		OnUpdate();
	}
}

void Application::Initialization()
{
}

void Application::OnGUI()
{
}

void Application::OnUpdate()
{
}


Application::~Application()
{
}
