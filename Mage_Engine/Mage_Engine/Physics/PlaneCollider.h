#pragma once
#include "Core/Component.h"
#include "Maths/Vector.h"

class PlaneCollider :
	public Component
{
public:
	PlaneCollider(Entity &entity);
	void Update(Application &app) override;
	void FixedUpdate(Application &app) override;
	~PlaneCollider();
	Mage::Maths::Vector3 m_normal;
	Mage::Maths::Vector3 m_position;
};

