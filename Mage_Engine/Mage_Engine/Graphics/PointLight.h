#pragma once
#include "Core/Component.h"
#include "Maths/Vector.h"

class Application;

class PointLight :
	public Component
{
public:
	PointLight(Entity &entity);
	void OnGUI(Application &app);
	Mage::Maths::Vector3 m_intensity;
	Mage::Maths::Vector3 m_position;
	float m_radius;
	~PointLight();
};

