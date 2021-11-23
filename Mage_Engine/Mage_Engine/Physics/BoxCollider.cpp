#include "Physics/BoxCollider.h"
#include "Core/Application.h"


BoxCollider::BoxCollider(Entity &entity) :
	Component(entity), m_center(Mage::Maths::Vector3(0, 0, 0)), m_minDimensions(Mage::Maths::Vector3(0, 0, 0)), m_maxDimensions(Mage::Maths::Vector3(0, 0, 0))
{
}

void BoxCollider::Update(Application & app)
{
}

void BoxCollider::FixedUpdate(Application & app)
{
}

void BoxCollider::OnGUI(Application & app)
{
	app.m_viz->GUIText("Box Collider");
	app.m_viz->GUIVector3("Center", m_center);
	app.m_viz->GUIVector3("Min", m_minDimensions);
	app.m_viz->GUIVector3("Max", m_maxDimensions);
}

BoxCollider::~BoxCollider()
{
}
