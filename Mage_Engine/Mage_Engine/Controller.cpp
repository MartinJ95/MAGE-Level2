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

BTStatus BTSelectorNode::Evaluate(Entity* e) const
{
    for (auto& n : m_children)
    {
        if (n.Evaluate(e) == BTStatus::eSuccess)
        {
            return BTStatus::eSuccess;
        }
    }
    return BTStatus::eFailure;
}

BTStatus BTSequenceNode::Evaluate(Entity* e) const
{
    for (auto& n : m_children)
    {
        if (n.Evaluate(e) != BTStatus::eSuccess)
        {
            return BTStatus::eFailure;
        }
    }
    return BTStatus::eSuccess;
}

BTStatus BehaviourTree::Evaluate(Entity* e) const
{
    return m_root.Evaluate(e);
}
