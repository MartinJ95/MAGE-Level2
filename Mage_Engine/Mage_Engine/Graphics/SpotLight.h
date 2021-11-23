#pragma once
#include "Core/Component.h"
#include "Maths/Vector.h"

class SpotLight :
	public Component
{
public:
	SpotLight(Entity &entity);
	void OnGUI(Application &app) override;
	Mage::Maths::Vector3 m_intensity;
	Mage::Maths::Vector3 m_position;
	float m_fieldOfView;
	float m_range;
	~SpotLight();
};

