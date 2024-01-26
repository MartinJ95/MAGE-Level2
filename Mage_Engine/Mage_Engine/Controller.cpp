#include "Controller.h"
#include "Core/Entity.h"
#include "Core/Application.h"
#include "Terrain.h"

Controller::Controller(Entity& entity, unsigned int ID, ComponentType type) : Component(&entity, ID, type)
{
}

Controller::~Controller()
{
}

AIController::AIController(Entity& entity, unsigned int ID, ComponentType type) :
    Controller(entity, ID, type), m_behaviourTree()
{
}

AIController::~AIController()
{
}

BTStatus BTSelectorNode::Evaluate(Entity* e)
{
    for (auto& n : m_children)
    {
        if (n->Evaluate(e) == BTStatus::eSuccess)
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
        if (n->Evaluate(e) != BTStatus::eSuccess)
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

DemoAIController::DemoAIController(Entity& entity, unsigned int ID, ComponentType type) :
    AIController(entity, ID, type)
{
    m_behaviourTree.m_root.m_children.emplace_back(new BTSequenceNode());
    BTSequenceNode* s = static_cast<BTSequenceNode*>(m_behaviourTree.m_root.m_children[0]);
    s->m_children.emplace_back(new AIMoveNode());
    s->m_children.emplace_back(new AIWaitNode());
}

void DemoAIController::Update(Application& app)
{
    m_behaviourTree.Evaluate(m_entity);
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

        DemoAIController* c = e->getComponent<DemoAIController>();

        if (c != nullptr)
        {
            AIMoveNode *n = static_cast<AIMoveNode*>(static_cast<BTSequenceNode*>(c->m_behaviourTree.m_root.m_children[0])->m_children[0]);
            if (n != nullptr)
            {
                n->targetPos = Mage::Maths::Vector3((float)(rand() % 100 - 50), 0.f, (float)(rand() % 100 - 50));
            }
        }

        return BTStatus::eSuccess;
    }
    return BTStatus::eFailure;
}

BTStatus AIMoveNode::Evaluate(Entity* e)
{
    Transform* t = e->getComponent<Transform>();

    if (t == nullptr)
        return BTStatus::eSuccess;

    Mage::Maths::Vector3 vec(targetPos - t->m_position);

    if (vec.Length() < 10.f)
        return BTStatus::eSuccess;

    t->m_position += vec.Normalized() * 2.f;

    Terrain* ter = e->getComponent<Terrain>();
    Application* app = ServiceLocator::GetMainService();

    if (ter == nullptr || app == nullptr)
        return BTStatus::eFailure;

    Mage::Maths::Vector3 pointOnTerrain = ter->GetPointOnTerrain(
        Mage::Maths::Vector2(t->m_position.x, t->m_position.y),
        *app);

    if (pointOnTerrain != Mage::Maths::Vector3(0.f, 0.f, 0.f))
    {
        t->m_position.y = fmaxf(t->m_position.y, pointOnTerrain.y);
    }

    return BTStatus::eFailure;
}
