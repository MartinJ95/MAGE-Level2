#include "Physics/PlaneCollider.h"
#include "RigidBody.h"
#include "SphereCollider.h"
#include "Core/Application.h"



PlaneCollider::PlaneCollider(Entity &entity) :
	Component(entity, 6),
	m_normal(Mage::Maths::Vector3(1, 0, 0)),
	m_position(Mage::Maths::Vector3(0, 0, 0))
{
}

void PlaneCollider::Update(Application & app)
{
	m_normal.NormalizeInPlace();
}

void PlaneCollider::FixedUpdate(Application & app)
{
}

void PlaneCollider::OnGUI(Application & app)
{
	app.m_viz->GUIText("PlaneCollider");
	app.m_viz->GUIVector3("Normal", m_normal);
	app.m_viz->GUIVector3("Position", m_position);
}


PlaneCollider::~PlaneCollider()
{
}
