#pragma once
#include "Core/Component.h"
#include "Maths/Vector.h"

class SphereCollider :
	public Component
{
public:
	SphereCollider(Entity &entity);
	void Update(Application &app) override;
	void FixedUpdate(Application &app) override;
	void OnGUI(Application &app) override;
	virtual void OnSave(const Application& app, std::ofstream& stream) const override;
	virtual void OnLoad(Application& app, std::ifstream& stream) override;
	~SphereCollider();
	float m_radius;
	Mage::Maths::Vector3 m_center;
};

