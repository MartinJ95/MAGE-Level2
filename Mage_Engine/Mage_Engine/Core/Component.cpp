#include "Component.h"
#include "Core/Application.h"
#include "Entity.h"


Component::Component(Entity &entity) :
	m_entity(entity)
{
}

void Component::Update(Application &app)
{
}

void Component::FixedUpdate(Application &app)
{
}

void Component::OnRender(Application & app)
{
}

void Component::OnGUI(Application & app)
{
}

void Component::OnSave(const Application & app, std::ofstream & stream)
{
}

void Component::OnLoad(Application & app, std::ifstream & stream)
{
}

void Component::onCollisionEnter(Application &app, collisionData & data)
{
}

Component::~Component()
{
	
}
