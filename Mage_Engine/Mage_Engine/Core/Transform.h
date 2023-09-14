#pragma once
#include "Core/Component.h"
#include "Maths/Vector.h"
#include "Maths/Quaternion.h"

class Entity;
class World;

class Transform :
	public Component
{
public:
	Transform(Entity &entity);
	void Update(Application &app) override;
	void FixedUpdate(Application &app) override;
	void OnGUI(Application &app) override;
	void Rotate(const Mage::Maths::Vector3& euler);
	void SetRotation(const Mage::Maths::Vector3& euler);
	void Rotate(const Mage::Maths::Quaternion& quat);
	void SetRotation(const Mage::Maths::Quaternion& quat);
	Mage::Maths::Vector3 worldForward();
	Mage::Maths::Vector3 worldPosition();
	Mage::Maths::Vector3 worldScale();
	Mage::Maths::Vector3 worldRotation();
	Mage::Maths::Quaternion worldRotationQuat();
	~Transform();
	Mage::Maths::Vector3 m_forward;
	Mage::Maths::Vector3 m_position;
	Mage::Maths::Vector3 m_scale;
	Mage::Maths::Vector3 m_rotation; //(vec3(pitch, yaw, roll))
	Mage::Maths::Quaternion m_quatRotation;
	void updateDirection();
private:
	void updateRotation(Application &app);
	void applyEulerToForward(const Mage::Maths::Vector3 &rotation, Mage::Maths::Vector3 &forward);
	void snapRotation(Mage::Maths::Vector3 &rotation);
};

