#pragma once
#include "Core/Component.h"
#include "Maths/Vector.h"
class PointLight :
	public Component
{
public:
	PointLight(Entity &entity);
	Mage::Maths::Vector3 m_intensity;
	Mage::Maths::Vector3 m_position;
	float m_radius;
	~PointLight();
};

