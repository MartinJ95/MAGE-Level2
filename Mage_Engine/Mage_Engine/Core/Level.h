#pragma once
#include <iostream>
#include <vector>

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