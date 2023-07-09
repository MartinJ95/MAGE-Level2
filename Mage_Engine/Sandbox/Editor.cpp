#include "pch.h"
#include "Editor.h"
#include <queue>
#include <stack>

Editor::Editor()
{
}

void Editor::Initialization()
{
	for (auto &t : textureList)
	{
		m_viz->generateTexture(t.first, t.second);
	}
}

void Editor::OnGUI()
{
	m_viz->GUIBegin("Hiearchy");
	m_viz->GUIVector3("world up", m_worldUp);
	m_viz->GUIVector3("world forward", m_worldForward);
	m_viz->GUIText("Current Level");
	m_viz->GUIText(m_currentLevel->m_levelName);
	m_viz->GUIEditText("level edit name", m_newLevelName);
	if (m_viz->GUIButton("Save Level")) { m_currentLevel->SaveLevel(*this); }
	if (m_viz->GUIButton("Load Level")) { m_currentLevel->LoadLevel(m_newLevelName, *this); }
	m_viz->GUIEditText("name for new entity", m_newEntityName);
	VoidFunctionCallbackString func = &Application::AddEntity;
	m_viz->GUIButton("Add New Entity", func, m_newEntityName, this);
	int entityDepth = 0;
	int entityIndex = 0;
	//std::queue<std::pair<Entity*, int>> entityQueue;
	std::vector<std::pair<Entity*, int>> sortedEntities;
	std::stack<int> previousIndex;
	int GUIIDIteration = 0;
	for (int i = 0; i < m_currentLevel->m_entities.size(); i++) 
	{
		Entity *e = m_currentLevel->m_entities[i];
		sortedEntities.push_back(std::pair<Entity*, int>(e, 0));
		while (e != nullptr)
		{
			if (e->m_children.empty() || e->m_children.size() <= entityIndex)
			{
				if (entityDepth > 0)
				{
					//entityQueue.push(std::pair<Entity*, int>(e, entityDepth));
					e = e->m_parent;
					entityDepth--;
					entityIndex = previousIndex.top();
					previousIndex.pop();
				}
				else
				{
					//entityQueue.push(std::pair<Entity*, int>(e, entityDepth));
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
	}
	/*while (!entityQueue.empty())
	{
		sortedEntities.push_back(entityQueue.front());
		entityQueue.pop();
		std::vector<std::pair<Entity*, int>>::reverse_iterator it = sortedEntities.rbegin();
		std::vector<std::pair<Entity*, int>>::reverse_iterator next;
		bool isDescendant = false;
		while (it != sortedEntities.rend()-1)
		{
			next = it + 1;
			Entity* e = next->first;
			while (e != nullptr)
			{
				if (e->m_parent == it->first) { isDescendant = true; break; }
				e = e->m_parent;
			}
			if (next->first->m_parent == it->first->m_parent && it->first->m_parent != nullptr || isDescendant)
			{
				std::swap(*it, *next);
				//next->swap(*it);
				it++;
				isDescendant = false;
			}
			else { break; }
		}
	}*/
	for (auto se : sortedEntities)
	{
		GUIIDIteration++;
		std::string s = "";
		for (int i = 0; i < se.second; i++)
		{
			s += "->";
		}
		if (m_viz->GUIButton(s + se.first->m_name + "##" + std::to_string(GUIIDIteration))) { SelectedEntity = se.first; }
	}
	GUIIDIteration = 0;
	/*while (!entityQueue.empty())
	{
		std::pair<Entity&, int> p = entityQueue.front();
		entityQueue.pop();
		std::string s = "";
		for (int i = 0; i < p.second; i++)
		{
			s += "->";
		}
		if (m_viz->GUIButton(s + p.first.m_name)) { SelectedEntity = &p.first; }
	}*/

	//for (int i = 0; i < m_currentLevel->m_entities.size(); i++)
	//{
	//	if (m_viz->GUIButton(m_currentLevel->m_entities[i]->m_name))
	//	{ SelectedEntity = m_currentLevel->m_entities[i]; }
	//}
	m_viz->GUIEnd();
	if (SelectedEntity != NULL)
	{
		m_viz->GUIBegin("Selected Entity");
		if(m_viz->GUIButton("Remove Entity"))
		{
			if (SelectedEntity->m_parent != nullptr)
			{
				for (std::vector<Entity>::iterator it = SelectedEntity->m_parent->m_children.begin(); it != SelectedEntity->m_parent->m_children.end(); it++)
				{
					if (&*it == SelectedEntity)
					{
						SelectedEntity->m_parent->m_children.erase(it);
						break;
					}
				}
			}
			else
			{
				for (std::vector<Entity*>::iterator it = m_currentLevel->m_entities.begin(); it != m_currentLevel->m_entities.end(); it++)
				{
					if (*it == SelectedEntity)
					{
						delete *it;
						m_currentLevel->m_entities.erase(it);
						break;
					}
				}
			}
			SelectedEntity = nullptr;
			m_viz->GUIEnd();
			/*for (int i = 0; i < m_currentLevel->m_entities.size(); i++)
			{
				if (m_currentLevel->m_entities[i] == SelectedEntity)
				{
					delete m_currentLevel->m_entities[i];
					m_currentLevel->m_entities.erase(m_currentLevel->m_entities.begin() + i);
					m_viz->GUIEnd();
					SelectedEntity = NULL;
				}
			}*/
		}
		else
		{
			SelectedEntity->OnGUI(*this);
			int index = 0;
			for (auto it : IncludeComponents)
			{
				if (m_viz->GUIButton("Add Component-" + it.first)) {
					EntityComponentAddition func = it.second;
					for (int i = 0; i < m_currentLevel->m_entities.size(); i++)
					{
						if (m_currentLevel->m_entities[i] == SelectedEntity) { index = i; break; }
					}
					(this->*func)(index);
				}
			}
			m_viz->GUIEnd();
		}
	}
}

void Editor::OnUpdate()
{
	m_viz->clear();
	glfwPollEvents();
	OnGUI();
	for (auto &e : m_currentLevel->m_entities)
	{
		Transform *t = e->getComponent<Transform>();
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
	std::cout << "new render from here" << std::endl;
	m_viz->display();
}


Editor::~Editor()
{
}