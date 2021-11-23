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
	~SphereCollider();
	float m_radius;
	Mage::Maths::Vector3 m_center;
};

