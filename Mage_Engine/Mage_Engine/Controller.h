#pragma once
#include "core/Component.h"

class Controller : public Component
{
public:
	Controller(Entity* entity, unsigned int ID = 0, ComponentType type = ComponentType::eDefaultComponent);
	~Controller();
};

class AIController : public Component
{
public:
	AIController(Entity* entity, unsigned int ID = 0, ComponentType type = ComponentType::eDefaultComponent);
	~AIController();
};

