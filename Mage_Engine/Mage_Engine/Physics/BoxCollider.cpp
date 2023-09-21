#include "Physics/BoxCollider.h"
#include "Core/Application.h"


BoxCollider::BoxCollider(Entity &entity) :
	Component(entity, 7), m_center(Mage::Maths::Vector3(0, 0, 0)), m_minDimensions(Mage::Maths::Vector3(0, 0, 0)), m_maxDimensions(Mage::Maths::Vector3(0, 0, 0))
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

void BoxCollider::OnSave(const Application& app, std::ofstream& stream) const
{
	Component::OnSave(app, stream);
	m_center.Save(stream);
	m_minDimensions.Save(stream);
	m_maxDimensions.Save(stream);
}

void BoxCollider::OnLoad(Application& app, std::ifstream& stream)
{
	m_center.Load(stream);
	m_minDimensions.Load(stream);
	m_maxDimensions.Load(stream);
}

BoxCollider::~BoxCollider()
{
}
