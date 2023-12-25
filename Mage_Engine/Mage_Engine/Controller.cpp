#include "Controller.h"
#include "Core/Entity.h"
#include "Core/Application.h"

Controller::Controller(Entity* entity, unsigned int ID, ComponentType type) : Component(entity, ID, type)
{
}

Controller::~Controller()
{
}

AIController::AIController(Entity* entity, unsigned int ID, ComponentType type) :
    Component(entity, ID, type), m_behaviourTree()
{
}

AIController::~AIController()
{
}

BTStatus BTSelectorNode::Evaluate(Entity* e)
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

BTStatus BTSequenceNode::Evaluate(Entity* e)
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

BTStatus BehaviourTree::Evaluate(Entity* e)
{
    return m_root.Evaluate(e);
}

DemoAIController::DemoAIController(Entity* entity, unsigned int ID, ComponentType type) :
    AIController(entity, ID, type)
{
}

void DemoAIController::Update(Application& app)
{
}

void DemoAIController::OnGUI(Application& app)
{
}

void DemoAIController::OnSave(const Application& app, std::ofstream& stream) const
{
}

void DemoAIController::OnLoad(Application& app, std::ifstream& stream)
{
}

void DemoAIController::onCollisionEnter(Application& app, collisionData& data)
{
}

BTStatus AIWaitNode::Evaluate(Entity* e) {
    Application* app = ServiceLocator::GetMainService();

    if (app == nullptr)
        return BTStatus::eFailure;

    waitTime -= app->m_deltaTime;

    if (waitTime < 0.f)
    {
        waitTime += rand() % 5 + 5.f;
        return BTStatus::eSuccess;
    }
    return BTStatus::eFailure;
}
