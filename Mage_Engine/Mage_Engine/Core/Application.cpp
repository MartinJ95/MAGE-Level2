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
	m_viz->generateShader("Resources\\default2DShader.vs", "Resources\\default2DShader.fs", "default2DShader");
	m_viz->generateShader("Resources\\default3DShader.vs", "Resources\\default3DShader.fs", "default3DShader");
	m_viz->generateBoxMesh(-1, 1, 0, 1, "box");
	m_viz->generateSquareMesh(-1, 1, 0, 1, "square");
	m_viz->generateSphereMesh(Mage::Maths::Vector3(0, 0, 0), 1, 50, "sphere");
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

void Application::AddEntity(const std::string &str)
{
	m_currentLevel->AddEntity(str);
}

void Application::RemoveEntity(const int & index)
{
	m_currentLevel->RemoveEntity(index);
}


Application::~Application()
{
	delete m_viz;
	delete m_currentLevel;
	delete m_physics;
}
