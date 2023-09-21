#pragma once
#include "Core/Component.h"
#include "maths/Vector.h"

class RigidBody :
	public Component
{
public:
	RigidBody(Entity &entity);
	void Update(Application &app) override;
	void FixedUpdate(Application &app) override;
	void OnGUI(Application &app) override;
	virtual void OnSave(const Application& app, std::ofstream& stream) const override;
	virtual void OnLoad(Application& app, std::ifstream& stream) override;
	~RigidBody();
	float m_mass;
	float m_restitution;
	Mage::Maths::Vector3 m_velocity;
	Mage::Maths::Vector3 m_force;
	Mage::Maths::Vector3 m_impulseForce;
};

