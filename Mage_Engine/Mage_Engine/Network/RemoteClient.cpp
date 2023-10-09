#include "RemoteClient.h"
#include "Core/Transform.h"
#include "Core/Entity.h"
#include <iostream>



RemoteClient::RemoteClient(Entity &entity) : 
	Component(&entity),
	m_ID(-1)
{
}

void RemoteClient::Update(Application & app)
{
	std::shared_ptr<Transform> t = std::shared_ptr<Transform>(m_entity->getComponent<Transform>());
	std::cout << "client : " << m_ID << "position : " << t->m_position.x << t->m_position.y << t->m_position.z << std::endl;

	//std::cout << "remote client is active" << std::endl;
}



RemoteClient::~RemoteClient()
{
}
