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

class Level
{
public:
	Level();
	Level(std::string &fileName, Application &app);
	void LoadLevel(const std::string &fileName, Application &app, Entity** selectedEntity = nullptr);
	void SaveLevel(const Application &app);
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