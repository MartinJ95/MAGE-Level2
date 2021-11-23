#pragma once
#include "TimeManager.h"
#include "Graphics/Visualization.h"
#include "Core/Level.h"
#include "Physics/Physics.h"

class Application
{
public:
	Application();
	void Run();
	virtual void Initialization();
	virtual void OnGUI();
	virtual void OnUpdate();
	void AddEntity(const std::string &str);
	void RemoveEntity(const int &index);
	virtual ~Application();
	bool m_open;
	float m_deltaTime;
	TimeManager m_time;
	Visualization *m_viz;
	Physics *m_physics;
	Level *m_currentLevel;
	Mage::Maths::Vector3 m_ambientLighting;
	Mage::Maths::Vector3 m_worldUp;
	Mage::Maths::Vector3 m_worldForward;
};