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
	void OnGUI(Application &app) override;
	virtual void OnSave(const Application& app, std::ofstream& stream) const override;
	virtual void OnLoad(Application& app, std::ifstream& stream) override;
	~PlaneCollider();
	Mage::Maths::Vector3 m_normal;
	Mage::Maths::Vector3 m_position;
};

