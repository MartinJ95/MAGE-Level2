#pragma once
#include "Core/Application.h"
#include "Core/Entity.h"

static const std::pair<std::string, std::string> textureList[] = {
	std::pair<std::string, std::string>("Resources\\box.png", "box"),
	std::pair<std::string, std::string>("Resources\\floor.png", "floor")
};

class Editor :
	public Application
{
public:
	Editor();
	void Initialization() override;
	void OnGUI() override;
	void OnUpdate() override;
	template<typename T>
	void AddComponent(const int &index);
	template<>
	void AddComponent<PointLight>(const int &index);
	~Editor();
	std::string m_newEntityName;
	std::string m_newLevelName;
	Entity *SelectedEntity;
	bool m_showComponentAddMenu{ false };
};

template<typename T>
inline void Editor::AddComponent(const int & index)
{
	if (m_currentLevel->m_entities[index]->getComponent<T>() == nullptr)
	{
		m_currentLevel->m_entities[index]->addComponent<T>();
	}
}

template<>
inline void Editor::AddComponent<PointLight>(const int & index)
{
	if (m_currentLevel->m_entities[index]->getComponent<PointLight>() == nullptr)
	{
		m_currentLevel->m_entities[index]->addComponent<PointLight>();
		m_currentLevel->m_pointLights.push_back(m_currentLevel->m_entities[index]->getComponent<PointLight>());
	}
}

typedef void(Editor::*EntityComponentAddition)(const int&);

static const std::unordered_map<std::string, EntityComponentAddition> IncludeComponents{
	std::pair<std::string, EntityComponentAddition>(std::string("Transform"), &Editor::AddComponent<Transform>), std::pair<std::string, EntityComponentAddition>(std::string("Camera"), &Editor::AddComponent<Camera>),
	std::pair<std::string, EntityComponentAddition>(std::string("RigidBody"), &Editor::AddComponent<RigidBody>), std::pair<std::string, EntityComponentAddition>(std::string("Mesh"), &Editor::AddComponent<Mesh>),
	std::pair<std::string, EntityComponentAddition>(std::string("BoxCollider"), &Editor::AddComponent<BoxCollider>), std::pair<std::string, EntityComponentAddition>(std::string("SphereCollider"), &Editor::AddComponent<SphereCollider>),
	std::pair<std::string, EntityComponentAddition>(std::string("PlaneCollider"), &Editor::AddComponent<PlaneCollider>), std::pair<std::string, EntityComponentAddition>(std::string("PointLight"), &Editor::AddComponent<PointLight>)
};