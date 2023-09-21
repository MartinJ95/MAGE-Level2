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
	void OnGUI(Application &app) override;
	virtual void OnSave(const Application& app, std::ofstream& stream) const override;
	virtual void OnLoad(Application& app, std::ifstream& stream) override;
	~BoxCollider();
	Mage::Maths::Vector3 m_center;
	Mage::Maths::Vector3 m_minDimensions;
	Mage::Maths::Vector3 m_maxDimensions;
};

