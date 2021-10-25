#include "Component.h"
#include "Application.h"
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

void Component::onCollisionEnter(Application &app, collisionData & data)
{
}

Component::~Component()
{
	
}
