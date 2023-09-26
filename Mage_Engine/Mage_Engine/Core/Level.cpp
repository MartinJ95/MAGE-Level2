#include "Level.h"
#include "Application.h"
#include <stack>

LevelData::LevelData() :
	m_levelName(), m_entities(), m_cameras(), m_pointLights(),
	m_spotLights(), m_mainCamera()
{}

LevelData::LevelData(LevelData & other) :
	m_levelName(other.m_levelName), m_entities(other.m_entities),
	m_cameras(other.m_cameras), m_pointLights(other.m_pointLights), 
	m_spotLights(other.m_spotLights), m_mainCamera(other.m_mainCamera)
{
	other.m_levelName.clear();
	other.m_entities.clear();
	other.m_cameras.clear();
	other.m_pointLights.clear();
	other.m_spotLights.clear();
	other.m_mainCamera = nullptr;
}


Level::Level() : m_data(), m_levelName(), m_newData()
{}

Level::Level(std::string &fileName, Application &app) : m_data(), m_levelName(), m_newData()
{
	LoadLevel(fileName, app);
}

void Level::LoadLevel(const std::string &fileName, Application &app, Entity** selectedEntity)
{
	UnloadLevel();
	if (*selectedEntity != nullptr)
		*selectedEntity = nullptr;
	m_newData.m_levelName = fileName;
	std::ifstream levelFile("Resources/" + m_levelName);
	if (levelFile.is_open())
	{
		app.m_ambientLighting.Load(levelFile);
		app.m_worldUp.Load(levelFile);
		app.m_worldForward.Load(levelFile);
		Entity* e{nullptr};
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
				if (e == nullptr) { continue; }
				e = &e->m_children.back();
			}
			else if (command == "entity")
			{
				m_newData.m_entities.emplace_back(new Entity(true));
				m_newData.m_entities.back()->OnLoad(app, levelFile);
			}
			levelFile >> command;
		}
	}
}

void Level::SaveLevel(const Application &app)
{
	std::ofstream levelFile("Resources/" + m_data.m_levelName);
	if (levelFile.is_open())
	{
		levelFile.clear();
		app.m_ambientLighting.Save(levelFile);
		app.m_worldUp.Save(levelFile);
		app.m_worldForward.Save(levelFile);
		for (int i = 0; i < m_data.m_entities.size(); i++)
		{
			m_data.m_entities[i]->OnSave(app, levelFile);
		}
		levelFile << "end\n";
	}
}

void Level::ApplyLoad()
{
	if (m_newData.m_levelName.empty()) { return; }
	m_data = m_newData;
}

Level::~Level()
{

}

void Level::UnloadLevel()
{
	for (int i = 0; i < m_data.m_entities.size(); i++)
	{
		delete m_data.m_entities[i];
	}
	m_data.m_entities.clear();
	m_data.m_cameras.clear();
	m_data.m_pointLights.clear();
	m_data.m_spotLights.clear();
}

