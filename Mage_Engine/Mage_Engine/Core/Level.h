#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "Entity.h"

class Entity;
class Component;
class Visualization;
class PointLight;
class SpotLight;
class Camera;
class Application;

struct LevelData
{
	LevelData();
	LevelData(LevelData& other);
	void operator=(LevelData& rhs)
	{
		m_levelName = rhs.m_levelName;
		m_entities = rhs.m_entities;
		m_cameras = rhs.m_cameras;
		m_pointLights = rhs.m_pointLights;
		m_spotLights = rhs.m_spotLights;
		if (rhs.m_mainCamera != nullptr)
		{
			m_mainCamera = rhs.m_mainCamera;
		}
		
		rhs.m_levelName.clear();
		rhs.m_entities.clear();
		rhs.m_cameras.clear();
		rhs.m_pointLights.clear();
		rhs.m_spotLights.clear();
	}
	std::string m_levelName;
	std::vector<Entity*> m_entities;
	std::vector<Camera*> m_cameras;
	std::vector<std::shared_ptr<PointLight>> m_pointLights;
	std::vector<std::shared_ptr<SpotLight>> m_spotLights;
	std::shared_ptr<Camera> m_mainCamera;
};

class Level
{
public:
	Level();
	Level(std::string &fileName, Application &app);
	void LoadLevel(const std::string &fileName, Application &app, Entity** selectedEntity = nullptr);
	void SaveLevel(const Application &app);
	void ApplyLoad();
	void AddEntity(const std::string &name) { m_data.m_entities.emplace_back(new Entity(true)); m_data.m_entities.back()->m_name = name; }
	void RemoveEntity(const int &index) { m_data.m_entities.erase(m_data.m_entities.begin() + index); }
	~Level();
public:
	LevelData m_data;
	std::string m_levelName;
private:
	LevelData m_newData;
private:
	void UnloadLevel();
};