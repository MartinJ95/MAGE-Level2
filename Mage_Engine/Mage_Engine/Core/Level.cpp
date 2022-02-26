#include "Level.h"
#include "Application.h"
#include <stack>

Level::Level() : m_levelName(), m_entities(), m_cameras(), m_pointLights(),
	m_spotLights(), m_mainCamera()
{}

Level::Level(std::string &fileName, Application &app) : m_levelName(), m_entities(), m_cameras(), m_pointLights(),
m_spotLights(), m_mainCamera()
{
	LoadLevel(fileName, app);
}

void Level::LoadLevel(const std::string &fileName, Application &app)
{
	m_levelName = fileName;
	std::ifstream levelFile("Resources/" + m_levelName);
	if (levelFile.is_open())
	{
		app.m_ambientLighting.Load(levelFile);
		app.m_worldUp.Load(levelFile);
		app.m_worldForward.Load(levelFile);
		Entity *e;
		std::string command;
		levelFile >> command;
		std::stack<int> entityChildIteration;
		while (true)
		{
			if (command == "end")
			{
				if (e == nullptr)
				{
					break;
				}
				else
				{
					if (e->m_parent == nullptr)
					{
						e == nullptr;
					}
					else
					{
						e = e->m_parent;
					}
				}
			}
			else if (command == "child")
			{
				e->createChild(true);
				e = e->m_children.end;
			}
			levelFile >> command;
		}
	}
}

void Level::SaveLevel(const Application &app)
{
	std::ofstream levelFile("Resources/" + m_levelName);
	if (levelFile.is_open())
	{
		levelFile.clear();
		app.m_ambientLighting.Save(levelFile);
		app.m_worldUp.Save(levelFile);
		app.m_worldForward.Save(levelFile);
		for (int i = 0; i < m_entities.size(); i++)
		{
			m_entities[i]->OnSave(app, levelFile);
		}
		levelFile << "end/n";
	}
}

Level::~Level()
{

}

void Level::UnloadLevel()
{

}