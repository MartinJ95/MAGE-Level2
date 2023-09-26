#include "Application.h"


Application::Application(bool isEditor) : m_time(), m_open(true), m_isEditor(isEditor)
{
}

void Application::Run()
{
	StackDebugging::GetInstance()->LogFunction("Application::Initialization");
	m_viz = new Visualization(1280, 720, "MAGE");
	m_viz->initialise();
	m_viz->InitialiseGUI();
	m_physics = new Physics();
	m_currentLevel = new Level();
	//m_currentLevel->m_entities.reserve(10);
	m_viz->generateShader("Resources\\default2DShader.vs", "Resources\\default2DShader.fs", "default2DShader");
	m_viz->generateShader("Resources\\default3DShader.vs", "Resources\\default3DShader.fs", "default3DShader");
	StackDebugging::GetInstance()->LogFunction("vizualization::generateBox");
	m_viz->generateBoxMesh(-1, 1, 0, 1, "box");
	StackDebugging::GetInstance()->PopFunction();
	StackDebugging::GetInstance()->LogFunction("vizualization::genrateSquare");
	m_viz->generateSquareMesh(-1, 1, 0, 1, "square");
	StackDebugging::GetInstance()->PopFunction();
	StackDebugging::GetInstance()->LogFunction("vizualization::generateSphere");
	m_viz->generateSphereMesh(Mage::Maths::Vector3(0, 0, 0), 1, 50, "sphere");
	StackDebugging::GetInstance()->PopFunction();
	Initialization();
	StackDebugging::GetInstance()->PopFunction();

	while (m_viz->isOpen())
	{
		m_viz->clear();
		glfwPollEvents();
		m_deltaTime = m_time.TimeStep();
		OnUpdate();
		OnGUI();
		OnPhysicsStep();
		OnRender();
		m_viz->display();

		m_currentLevel->ApplyLoad();
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

void Application::OnRender()
{
	for (auto& e : m_currentLevel->m_data.m_entities)
	{
		Transform* t = e->getComponent<Transform>();
		if (t != NULL)
		{
			if (t->m_forward == Mage::Maths::Vector3(0, 0, 0))
			{
				t->m_forward = m_worldForward;
			};
			t->updateDirection();
		}
		e->OnRender(*this);
	}
}

void Application::OnPhysicsStep()
{
	for (auto& e : m_currentLevel->m_data.m_entities)
	{
		e->OnPhysicsStep(*this);
	}
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
	StackDebugging::DestroyInstance();
}
