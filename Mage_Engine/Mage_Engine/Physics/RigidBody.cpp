#include "RigidBody.h"
#include "Core/Application.h"


RigidBody::RigidBody(Entity &entity) : Component(entity),
	m_mass(1),
	m_restitution(0.9),
	m_velocity(0, 0, 0),
	m_force(0, 0, 0),
	m_impulseForce(0, 0, 0)
{

}

void RigidBody::Update(Application &app)
{
	app.m_physics->applyForces(app, *this);
	app.m_physics->handleCollisions(m_entity, app);
}

void RigidBody::FixedUpdate(Application &app)
{

}

void RigidBody::OnGUI(Application & app)
{
	app.m_viz->GUIText("Rigid Body");
	app.m_viz->GUIEditFloat("Mass", m_mass);
	app.m_viz->GUIEditFloat("Restitution", m_restitution);
}


RigidBody::~RigidBody()
{
}