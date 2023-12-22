#include "pch.h"
#include "Editor.h"
#include <queue>
#include <stack>
#include "EditorCam.h"

Editor::Editor() : Application(true), m_newEntityName(), m_newLevelName(), m_editorCam(true), SelectedEntity(nullptr), m_showComponentAddMenu(false)
{
}

void Editor::Initialization()
{
	m_editorCam.addComponent<Transform>();
	m_editorCam.addComponent<Camera>();
	m_editorCam.addComponent<EditorCam>();
	m_editorCam.getComponent<EditorCam>()->Initialize(*this);
	m_currentLevel->m_data.m_mainCamera = m_editorCam.getComponent<Camera>();

	for (auto &t : textureList)
	{
		m_viz->generateTexture(t.first, t.second);
	}
}

void Editor::MainLoopStart()
{
	if (m_isRunning)
	{
		m_runningThreads.emplace(&Application::OnUpdate, &*this);
		m_runningThreads.emplace(&Application::OnPhysicsStep, &*this);
		//OnUpdate();
		//OnPhysicsStep();
	}
	m_runningThreads.emplace(&Editor::OnGUI, &*this);
	//m_runningThreads.emplace(&Application::OnRender, &*this);
	OnRender();
	//OnGUI();
	//OnRender();
}

void Editor::OnGUI()
{
	if (m_isRunning)
	{
		m_viz->GetGUI().GUIBegin("Hiearchy");
		if (m_viz->GetGUI().GUIButton("stop playing"))
		{
			m_isRunning = false;
		}
		m_viz->GetGUI().GUIEnd();
		return;
	}

	m_viz->GetGUI().GUIBegin("editorCam");
	m_editorCam.OnGUI(*this);
	m_viz->GetGUI().GUIEnd();

	m_viz->GetGUI().GUIBegin("Hiearchy");
	if (m_viz->GetGUI().GUIButton("play level"))
	{
		m_isRunning = true;
		SelectedEntity = nullptr;
	}
	m_viz->GetGUI().GUIVector3("ambient lighting", m_ambientLighting);
	m_viz->GetGUI().GUIVector3("world up", m_worldUp);
	m_viz->GetGUI().GUIVector3("world forward", m_worldForward);
	m_viz->GetGUI().GUIText("Current Level");
	m_viz->GetGUI().GUIText(m_currentLevel->m_levelName);
	m_viz->GetGUI().GUIEditText("level edit name", m_newLevelName);
	if (m_viz->GetGUI().GUIButton("Save Level")) { m_currentLevel->SaveLevel(*this); }
	if (m_viz->GetGUI().GUIButton("Load Level")) { m_currentLevel->LoadLevel(m_newLevelName, *this, &SelectedEntity); }
	m_viz->GetGUI().GUIEditText("name for new entity", m_newEntityName);
	VoidFunctionCallbackString func = &Application::AddEntity;
	m_viz->GetGUI().GUIButton("Add New Entity", func, m_newEntityName, this);
	int entityDepth = 0;
	int entityIndex = 0;
	std::vector<std::pair<Entity*, int>> sortedEntities;
	std::stack<int> previousIndex;
	int GUIIDIteration = 0;
	for (int i = 0; i < m_currentLevel->m_data.m_entities.size(); i++) 
	{
		GetLock(i).lock();
		Entity *e = m_currentLevel->m_data.m_entities[i];
		sortedEntities.push_back(std::pair<Entity*, int>(e, 0));
		while (e != nullptr)
		{
			if (e->m_children.empty() || e->m_children.size() <= entityIndex)
			{
				if (entityDepth > 0)
				{
					e = e->m_parent;
					entityDepth--;
					entityIndex = previousIndex.top();
					previousIndex.pop();
				}
				else
				{
					e = nullptr;
				}
			}
			else
			{
				previousIndex.push(entityIndex + 1);
				entityDepth++;
				e = &e->m_children[entityIndex];
				entityIndex = 0;
				sortedEntities.push_back(std::pair<Entity*, int>(e, entityDepth));
			}
		}
		GetLock(i).unlock();
	}
	for (auto se : sortedEntities)
	{
		GUIIDIteration++;
		std::string s = "";
		for (int i = 0; i < se.second; i++)
		{
			s += "->";
		}
		if (m_viz->GetGUI().GUIButton(s + se.first->m_name + "##" + std::to_string(GUIIDIteration))) { SelectedEntity = se.first; }
	}
	GUIIDIteration = 0;
	m_viz->GetGUI().GUIEnd();
	if (SelectedEntity != NULL)
	{
		m_viz->GetGUI().GUIBegin("Selected Entity");
		if(m_viz->GetGUI().GUIButton("Remove Entity"))
		{
			/*if (SelectedEntity->m_parent != nullptr)
			{
				for (std::vector<Entity>::iterator it = SelectedEntity->m_parent->m_children.begin(); it != SelectedEntity->m_parent->m_children.end(); it++)
				{
					if (&*it == SelectedEntity)
					{
						//SelectedEntity->m_parent->m_children.erase(it);
						break;
					}
				}
			}
			else
			{
				for (std::vector<Entity*>::iterator it = m_currentLevel->m_data.m_entities.begin(); it != m_currentLevel->m_data.m_entities.end(); it++)
				{
					if (*it == SelectedEntity)
					{
						//delete *it;
						//m_currentLevel->m_data.m_entities.erase(it);
						break;
					}
				}
			}*/
			SelectedEntity->m_markedForDeletion = true;
			SelectedEntity = nullptr;
			m_viz->GetGUI().GUIEnd();
		}
		else
		{
			SelectedEntity->OnGUI(*this);
			int index = 0;
			for (auto it : IncludeComponents)
			{
				if (m_viz->GetGUI().GUIButton("Add Component-" + it.first)) {
					EntityComponentAddition func = it.second;
					for (int i = 0; i < m_currentLevel->m_data.m_entities.size(); i++)
					{
						if (m_currentLevel->m_data.m_entities[i] == SelectedEntity) { index = i; break; }
					}
					m_componentsToAdd.push(std::pair<EntityComponentAddition, int>(func, index));
					//(this->*func)(index);
				}
			}
			m_viz->GetGUI().GUIEnd();
		}
	}

	m_editorCam.Update(*this);
}

void Editor::OnFrameEnd()
{
	Application::OnFrameEnd();

	while (!m_componentsToAdd.empty())
	{
		std::pair<EntityComponentAddition, int> func = m_componentsToAdd.front();
		m_componentsToAdd.pop();
		(this->*func.first)(func.second);
	}
}

Editor::~Editor()
{
}

