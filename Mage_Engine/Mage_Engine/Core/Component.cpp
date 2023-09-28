#include "Component.h"
#include "Core/Application.h"
#include "Entity.h"


Component::Component(Entity &entity, unsigned int ID, ComponentType type) :
	m_entity(entity), compID(ID), compType(type), m_markedForDeletion(false)
{
}

void Component::OnStart(Application& app)
{
}

void Component::Update(Application &app)
{
}

void Component::FixedUpdate(Application &app)
{
}

void Component::OnRender(Application & app) const
{
}

void Component::OnGUI(Application & app)
{
}

void Component::OnSave(const Application & app, std::ofstream & stream) const
{
	stream << "component" << "\n";
	stream << compID << "\n";
}

void Component::OnLoad(Application & app, std::ifstream & stream)
{
}

void Component::onCollisionEnter(Application &app, collisionData & data)
{
}

void Component::Cleanup()
{
	if (!m_markedForDeletion) { return; }

	for (int i = m_entity.m_components.size()-1; i >= 0; i--)
	{
		if (m_entity.m_components[i] == this)
		{
			m_entity.m_components.erase(m_entity.m_components.begin() + i);
		}
	}
	
	/*for (std::vector<Component*>::reverse_iterator it = m_entity.m_components.rbegin(); it != m_entity.m_components.rend(); it++)
	{
		if (*it == this)
		{
			m_entity.m_components.erase(std::next(it).base());
		}
	}*/
}

Component::~Component()
{
	
}
