#include "Application.h"


Application::Application() : m_open(true), m_time()
{
}

void Application::Run()
{
	m_viz = new Visualization(1280, 720, "MAGE");
	m_viz->initialise();
	m_viz->InitialiseGUI();
	m_physics = new Physics();
	m_currentLevel = new Level();
	Initialization();
	while (m_viz->isOpen())
	{
		m_deltaTime = m_time.TimeStep();
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
	delete m_viz;
	delete m_currentLevel;
	delete m_physics;
}
