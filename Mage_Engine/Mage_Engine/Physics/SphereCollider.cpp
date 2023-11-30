#include "SphereCollider.h"
#include "Core/Application.h"


SphereCollider::SphereCollider(Entity &entity) :
	Component(&entity, 8, ComponentType::ePhysicsComponent),
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
	app.m_viz->GetGUI().GUIText("Sphere Collider");
	app.m_viz->GetGUI().GUIEditFloat("Radius", m_radius);
	app.m_viz->GetGUI().GUIVector3("m_center", m_center);
}

void SphereCollider::OnSave(const Application& app, std::ofstream& stream) const
{
	Component::OnSave(app, stream);
	stream << m_radius << "\n";
	m_center.Save(stream);
}

void SphereCollider::OnLoad(Application& app, std::ifstream& stream)
{
	stream >> m_radius;
	m_center.Load(stream);
}


SphereCollider::~SphereCollider()
{
}
