#pragma once
#include "TimeManager.h"
#include "Graphics/Visualization.h"
#include "Core/Level.h"
#include "Physics/Physics.h"
#include "StackDebugging.h"
#include <thread>
#include <mutex>

class MainService
{

};

class GraphicsService
{

};

class PhyscicsService
{

};

class ServiceLocator
{
public:
	static void Initialize(Application& app)
	{
		m_app = &app;
		m_viz = app.m_viz;
		m_physics = app.m_physics;
	}
	static Application* GetMainService()
	{
		return m_app;
	}
	static Visualization* GetGraphicsService()
	{
		return m_viz;
	}
	static Physics* GetPhysicsService()
	{
		return m_physics;
	}


private:
	static Application* m_app;
	static Visualization* m_viz;
	static Physics* m_physics;
};

class Application
{
public:
	Application(bool isEditor = false);
	void Run();
	virtual void Initialization();
	virtual void OnGUI();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnPhysicsStep();
	void AddEntity(const std::string &str);
	void RemoveEntity(const int &index);
	virtual ~Application();
	float m_deltaTime;
	TimeManager m_time;
	Visualization *m_viz;
	Physics *m_physics;
	Level *m_currentLevel;
	Mage::Maths::Vector3 m_ambientLighting;
	Mage::Maths::Vector3 m_worldUp;
	Mage::Maths::Vector3 m_worldForward;
	static Application* Instance;
	std::stack<std::thread> m_runningThreads;
	std::mutex m_locks[10];
	bool m_open;
	bool m_isEditor;
	bool m_isRunning;
	bool m_lastRunningState;
protected:
	inline std::mutex& GetLock(int i) { return m_locks[i % 10]; }
	virtual void MainLoopStart();
	void MainLoopEnd();
	virtual void OnFrameEnd();
};