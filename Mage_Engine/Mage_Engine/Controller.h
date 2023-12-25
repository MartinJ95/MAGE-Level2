#pragma once
#include "core/Component.h"
#include <vector>

enum class BTStatus
{
	eSuccess,
	eFailure,
	eRunning
};

struct BTNode
{
	virtual BTStatus Evaluate(Entity* e) const = 0;
};

struct BTSelectorNode : public BTNode
{
	virtual BTStatus Evaluate(Entity* e) const override;
	std::vector<BTNode> m_children;
};

struct BTSequenceNode : public BTSelectorNode
{
	virtual BTStatus Evaluate(Entity* e) const override;
};

struct BehaviourTree
{
	BTStatus Evaluate(Entity *e) const;
	BTSelectorNode m_root;
};

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
	BehaviourTree m_behaviourTree;
};

class DemoAIController : public AIController
{
public:
	DemoAIController(Entity* entity, unsigned int ID = 0, ComponentType type = ComponentType::eDefaultComponent);
	virtual void Update(Application& app) override;
	virtual void OnGUI(Application& app) override;
	virtual void OnSave(const Application& app, std::ofstream &stream) const override;
	virtual void OnLoad(Application& app, std::ifstream &stream) override;
	virtual void onCollisionEnter(Application& app, collisionData& data);
};
