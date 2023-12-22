#include "Controller.h"

Controller::Controller(Entity* entity, unsigned int ID, ComponentType type) : Component(entity, ID, type)
{
}

Controller::~Controller()
{
}

AIController::AIController(Entity* entity, unsigned int ID, ComponentType type)
{
}

AIController::~AIController()
{
}
