#include "Physics/PlaneCollider.h"
#include "RigidBody.h"
#include "SphereCollider.h"
#include "Core/Application.h"



PlaneCollider::PlaneCollider(Entity &entity) :
	Component(entity, 6, ComponentType::ePhysicsComponent),
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

void PlaneCollider::OnSave(const Application& app, std::ofstream& stream) const
{
	Component::OnSave(app, stream);
	m_normal.Save(stream);
	m_position.Save(stream);
}

void PlaneCollider::OnLoad(Application& app, std::ifstream& stream)
{
	m_normal.Load(stream);
	m_position.Load(stream);
}


PlaneCollider::~PlaneCollider()
{
}
