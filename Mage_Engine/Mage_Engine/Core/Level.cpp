#include "Level.h"
#include "Application.h"

Level::Level() : m_levelName(), m_entities(), m_cameras(), m_pointLights(),
	m_spotLights(), m_mainCamera()
{}

Level::Level(std::string &fileName) : m_levelName(), m_entities(), m_cameras(), m_pointLights(),
m_spotLights(), m_mainCamera()
{
	LoadLevel(fileName);
}

void Level::LoadLevel(const std::string &fileName)
{
	m_levelName = fileName;
}

void Level::SaveLevel()
{

}

Level::~Level()
{

}

void Level::UnloadLevel()
{

}