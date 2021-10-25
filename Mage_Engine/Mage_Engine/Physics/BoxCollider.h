#pragma once
#include "Core/Component.h"
#include "Maths/Vector.h"

class BoxCollider :
	public Component
{
public:
	BoxCollider(Entity &entity);
	void Update(Application &app) override;
	void FixedUpdate(Application &app) override;
	~BoxCollider();
	Mage::Maths::Vector3 m_center;
	Mage::Maths::Vector3 m_minDimensions;
	Mage::Maths::Vector3 m_maxDimensions;
};

