#include "Application.h"


Application::Application(bool isEditor) : m_time(), m_open(true), m_isEditor(isEditor), m_isRunning(false)
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
		
		MainLoopStart();
		MainLoopEnd();

		m_viz->display();

		OnFrameEnd();
		m_currentLevel->ApplyLoad();
	}
}

void Application::Initialization()
{
}

void Application::MainLoopStart()
{
	if (m_isRunning)
	{
		m_runningThreads.emplace(&Application::OnUpdate, &*this);
		m_runningThreads.emplace(&Application::OnPhysicsStep, &*this);
		//OnUpdate();
		//OnPhysicsStep();
	}
	m_runningThreads.emplace(&Application::OnGUI, &*this);
	//m_runningThreads.emplace(&Application::OnRender, &*this);
	OnRender();
	//OnGUI();
}

void Application::MainLoopEnd()
{
	while (!m_runningThreads.empty())
	{
		m_runningThreads.top().join();
		m_runningThreads.pop();
	}
}

void Application::OnFrameEnd()
{
	if (m_currentLevel->m_data.m_entities.empty()) { return; }
	for (int i = m_currentLevel->m_data.m_entities.size()-1; i >= 0; i--)
	{
		Entity* e = m_currentLevel->m_data.m_entities[i];
		Entity* r = e->Cleanup(*this);
		if (r != nullptr)
		{
			m_currentLevel->m_data.m_entities.erase(m_currentLevel->m_data.m_entities.begin() + i);
			delete r;
		}
	}
	/*
	for (std::vector<Entity*>::reverse_iterator it = m_currentLevel->m_data.m_entities.rbegin(); it != m_currentLevel->m_data.m_entities.rend(); ++it)
	{
		Entity* e = nullptr;
		e = *it;
		Entity* r = e->Cleanup(*this);
		if (r != nullptr)
		{
			m_currentLevel->m_data.m_entities.erase(std::next(it).base());
			delete r;
			if (m_currentLevel->m_data.m_entities.empty()) { break; }
		}
	}*/
}

void Application::OnGUI()
{
}

void Application::OnUpdate()
{	
	for (int i = 0; i < m_currentLevel->m_data.m_entities.size(); i++)
	{
		GetLock(i).lock();
		auto& e = m_currentLevel->m_data.m_entities[i];
		e->Update(*this);
		GetLock(i).unlock();
	}
}

void Application::OnRender()
{
	for (int i = 0; i < m_currentLevel->m_data.m_entities.size(); i++)
	{
		GetLock(i).lock();
		auto& e = m_currentLevel->m_data.m_entities[i];
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
		GetLock(i).unlock();
	}
}

void Application::OnPhysicsStep()
{
	for (int i = 0; i < m_currentLevel->m_data.m_entities.size(); i++)
	{
		GetLock(i).lock();
		auto& e = m_currentLevel->m_data.m_entities[i];
		e->OnPhysicsStep(*this);
		GetLock(i).unlock();
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
