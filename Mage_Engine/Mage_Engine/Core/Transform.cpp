#include "Transform.h"
#include "Entity.h"
#include "Core/Application.h"

Transform::Transform(Entity& entity) :
	Component(&entity, 2),
	m_forward(0, 0, 1),
	m_position(0, 0, 0),
	m_scale(1, 1, 1),
	m_rotation(0, 0, 0)
{

}

void Transform::Update(Application & app)
{
}

void Transform::FixedUpdate(Application & app)
{
}

void Transform::OnGUI(Application & app)
{
	app.m_viz->GUIText("Transform");
	app.m_viz->GUIVector3("position", m_position);
	app.m_viz->GUIVector3("rotation", m_rotation);
	app.m_viz->GUIVector3("scale", m_scale);
	app.m_viz->GUIVector3("forward", m_forward);
}

void Transform::OnSave(const Application& app, std::ofstream& stream) const
{
	Component::OnSave(app, stream);
	m_forward.Save(stream);
	m_position.Save(stream);
	m_scale.Save(stream);
	m_rotation.Save(stream);
	m_quatRotation.Save(stream);
}

void Transform::OnLoad(Application& app, std::ifstream& stream)
{
	m_forward.Load(stream);
	m_position.Load(stream);
	m_scale.Load(stream);
	m_rotation.Load(stream);
	m_quatRotation.Load(stream);
}

void Transform::updateDirection()
{
	Mage::Maths::Vector3 forward(Mage::Maths::Vector3(0, 0, 1));
	Mage::Maths::Vector3 rotation = worldRotation();
	snapRotation(rotation);
	snapRotation(m_rotation);
	//applyEulerToForward(m_rotation, m_forward);
	m_forward = (m_quatRotation * forward).Normalized();
}

void Transform::updateRotation(Application &app)
{
	m_rotation.x = m_forward.AngleBetween(app.m_worldUp) - 90;
	m_rotation.y = m_forward.AngleBetween(app.m_worldForward);
	// todo
	// learn how to properly transform a direction to euler angles
	// improve this to find rotation values from a direction
}

void Transform::snapRotation(Mage::Maths::Vector3 & rotation)
{
	if (rotation.x > 89) rotation.x = 89;
	else if (rotation.x < -89) rotation.x = -89;

	while (rotation.y > 180) rotation.y -= 360;
	while (rotation.y < -180) rotation.y += 360;

	while (rotation.z > 180) rotation.z -= 360;
	while (rotation.z < -180) rotation.z += 360;
}

void Transform::applyEulerToForward(const Mage::Maths::Vector3 & rotation, Mage::Maths::Vector3 & forward)
{
	//forward.z = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
	//forward.y = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
	//forward.x = sin(glm::radians(rotation.y));
	//forward.x = sin(glm::radians(rotation.y));
	//forward.y = cos(glm::radians(rotation.x));
	//forward.z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
	
	//adjusts rotation so theta of 0 points to minus 1 z instead of positive x
	Mage::Maths::Vector3 adjustedRotation = rotation + Mage::Maths::Vector3(0, -90, 0);
	forward.x = cos(glm::radians(rotation.y) * cos(glm::radians(rotation.x)));
	forward.y = sin(glm::radians(rotation.x));
	forward.z = sin(glm::radians(rotation.y) * cos(glm::radians(rotation.x)));
	forward.NormalizeInPlace();
}

void Transform::Rotate(const Mage::Maths::Vector3& euler)
{
	Mage::Maths::Vector3 newEuler = euler + m_rotation;
	snapRotation(newEuler);
	SetRotation(newEuler);
}

void Transform::SetRotation(const Mage::Maths::Vector3& euler)
{
	m_rotation = euler;
	m_quatRotation = Mage::Maths::EulerToQuat(m_rotation);
	applyEulerToForward(m_rotation, m_forward);
}

void Transform::Rotate(const Mage::Maths::Quaternion& quat)
{
	Mage::Maths::Quaternion newQuaternion = quat;
	newQuaternion = newQuaternion * m_quatRotation;
	SetRotation(newQuaternion);
}

void Transform::SetRotation(const Mage::Maths::Quaternion& quat)
{
	m_quatRotation = quat;
	m_rotation = Mage::Maths::QuatToEuler(m_quatRotation);
	updateDirection();
}

Mage::Maths::Vector3 Transform::worldForward()
{
	Mage::Maths::Vector3 forward(Mage::Maths::Vector3(0, 0, 1));
	Mage::Maths::Vector3 rotation = worldRotation();
	snapRotation(rotation);
	applyEulerToForward(rotation, forward);
	//return (m_quatRotation * forward).Normalized();
	return forward;
}

Mage::Maths::Vector3 Transform::worldPosition()
{
	if (m_entity->m_parent != nullptr)
	{
		return m_position + m_entity->m_parent->getComponent<Transform>()->worldPosition();
	}
	else
	{
		return m_position;
	}
}

Mage::Maths::Vector3 Transform::worldScale()
{
	if (m_entity->m_parent != nullptr)
	{
		return m_scale + m_entity->m_parent->getComponent<Transform>()->worldScale();
	}
	else
	{
		return m_scale;
	}
}

Mage::Maths::Vector3 Transform::worldRotation()
{
	if (m_entity->m_parent != nullptr)
	{
		return m_rotation + m_entity->m_parent->getComponent<Transform>()->worldRotation();
	}
	else
	{
		return m_rotation;
	}
}

Mage::Maths::Quaternion Transform::worldRotationQuat()
{
	if (m_entity->m_parent != nullptr)
	{
		return m_quatRotation * m_entity->m_parent->getComponent<Transform>()->worldRotationQuat();
	}
	else
	{
		return m_quatRotation;
	}
}

Transform::~Transform()
{

}
