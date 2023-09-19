#pragma once
#include <fstream>

class Application;
class Entity;
struct collisionData;

class Component
{
public:
	Component(Entity &entity, unsigned int ID = 0);
	virtual void Update(Application &app);
	virtual void FixedUpdate(Application &app);
	virtual void OnRender(Application &app);
	virtual void OnGUI(Application &app);
	virtual void OnSave(const Application &app, std::ofstream &stream);
	virtual void OnLoad(Application &app, std::ifstream &stream);
	virtual void onCollisionEnter(Application &app, collisionData &data);
	virtual ~Component();
public:
	Entity &m_entity;
	unsigned int compID;
};