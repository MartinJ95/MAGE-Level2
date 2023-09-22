#pragma once
#include <fstream>

class Application;
class Entity;
struct collisionData;

enum class ComponentType
{
	eDefaultComponent = 0,
	ePhysicsComponent = 1,
	eGraphicsComponent = 2
};

class Component
{
public:
	Component(Entity &entity, unsigned int ID = 0, ComponentType type = ComponentType::eDefaultComponent);
	virtual void Update(Application &app);
	virtual void FixedUpdate(Application &app);
	virtual void OnRender(Application &app) const;
	virtual void OnGUI(Application &app);
	virtual void OnSave(const Application &app, std::ofstream &stream) const;
	virtual void OnLoad(Application &app, std::ifstream &stream);
	virtual void onCollisionEnter(Application &app, collisionData &data);
	virtual ~Component();
public:
	Entity &m_entity;
	unsigned int compID;
	ComponentType compType;
};