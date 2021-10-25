#pragma once
#include "Core/Component.h"
class RemoteClient : 
	public Component
{
public:
	RemoteClient(Entity &entity);
	void Update(Application &app) override;
	~RemoteClient();
public:
	int m_ID;
};

