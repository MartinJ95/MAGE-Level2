#include "Transform.h"
#include "Entity.h"
#include "Core/Application.h"

Transform::Transform(Entity& entity) :
	Component(entity),
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

void Transform::updateDirection()
{
	snapRotation(m_rotation);
	applyEulerToForward(m_rotation, m_forward);
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
	forward.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	forward.y = sin(glm::radians(rotation.x));
	forward.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	forward.NormalizeInPlace();
}

Mage::Maths::Vector3 Transform::worldForward()
{
	Mage::Maths::Vector3 forward;
	Mage::Maths::Vector3 rotation = worldRotation();
	snapRotation(rotation);
	applyEulerToForward(rotation, forward);
	return forward;
}

Mage::Maths::Vector3 Transform::worldPosition()
{
	if (m_entity.m_parent != nullptr)
	{
		return m_position + m_entity.m_parent->getComponent<Transform>()->worldPosition();
	}
	else
	{
		return m_position;
	}
}

Mage::Maths::Vector3 Transform::worldScale()
{
	if (m_entity.m_parent != nullptr)
	{
		return m_scale + m_entity.m_parent->getComponent<Transform>()->worldScale();
	}
	else
	{
		return m_scale;
	}
}

Mage::Maths::Vector3 Transform::worldRotation()
{
	if (m_entity.m_parent != nullptr)
	{
		return m_rotation + m_entity.m_parent->getComponent<Transform>()->worldRotation();
	}
	else
	{
		return m_rotation;
	}
}

Transform::~Transform()
{

}
