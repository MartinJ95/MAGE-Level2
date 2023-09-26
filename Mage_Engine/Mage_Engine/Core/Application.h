#pragma once
#include "TimeManager.h"
#include "Graphics/Visualization.h"
#include "Core/Level.h"
#include "Physics/Physics.h"
#include "StackDebugging.h"
#include <thread>
#include <mutex>

class Application
{
public:
	Application(bool isEditor = false);
	void Run();
	virtual void Initialization();
	virtual void OnGUI();
	virtual void OnUpdate();
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
	bool m_open;
	bool m_isEditor;
};