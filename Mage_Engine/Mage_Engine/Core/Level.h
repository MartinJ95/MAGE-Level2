#pragma once
#include <iostream>
#include <vector>
#include "Entity.h"

class Entity;
class Component;
class Visualization;
class PointLight;
class SpotLight;
class Camera;

class Level
{
public:
	Level();
	Level(std::string &fileName);
	void LoadLevel(const std::string &fileName);
	void SaveLevel();
	void AddEntity(const std::string &name) { m_entities.emplace_back(new Entity(true)); m_entities.back()->m_name = name; }
	void RemoveEntity(const int &index) { m_entities.erase(m_entities.begin() + index); }
	~Level();
	std::string m_levelName;
	std::vector<Entity*> m_entities;
	std::vector<Camera*> m_cameras;
	std::vector<PointLight*> m_pointLights;
	std::vector<SpotLight*> m_spotLights;
	Camera *m_mainCamera;
private:
	void UnloadLevel();
};