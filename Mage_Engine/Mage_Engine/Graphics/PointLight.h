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
	virtual void OnSave(const Application& app, std::ofstream& stream) const override;
	virtual void OnLoad(Application& app, std::ifstream& stream) override;
	Mage::Maths::Vector3 m_intensity;
	Mage::Maths::Vector3 m_position;
	float m_radius;
	~PointLight();
};

