#pragma once
#include "Core/Component.h"
#include "Maths/Vector.h"

class SpotLight :
	public Component
{
public:
	SpotLight(Entity &entity);
	void OnGUI(Application &app) override;
	virtual void OnSave(const Application& app, std::ofstream& stream) const override;
	virtual void OnLoad(Application& app, std::ifstream& stream) override;
	Mage::Maths::Vector3 m_intensity;
	Mage::Maths::Vector3 m_position;
	float m_fieldOfView;
	float m_range;
	~SpotLight();
};

