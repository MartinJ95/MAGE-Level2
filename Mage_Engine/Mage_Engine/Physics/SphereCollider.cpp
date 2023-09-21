#include "SphereCollider.h"
#include "Core/Application.h"


SphereCollider::SphereCollider(Entity &entity) :
	Component(entity, 8),
	m_radius(0.5),
	m_center(Mage::Maths::Vector3(0, 0, 0))
{
}

void SphereCollider::Update(Application &app)
{
}

void SphereCollider::FixedUpdate(Application & app)
{
}

void SphereCollider::OnGUI(Application & app)
{
	app.m_viz->GUIText("Sphere Collider");
	app.m_viz->GUIEditFloat("Radius", m_radius);
	app.m_viz->GUIVector3("m_center", m_center);
}


SphereCollider::~SphereCollider()
{
}
