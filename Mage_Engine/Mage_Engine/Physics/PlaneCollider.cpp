#include "Physics/PlaneCollider.h"
#include "RigidBody.h"
#include "SphereCollider.h"



PlaneCollider::PlaneCollider(Entity &entity) :
	Component(entity),
	m_normal(Mage::Maths::Vector3(1, 0, 0)),
	m_position(Mage::Maths::Vector3(0, 0, 0))
{
}

void PlaneCollider::Update(Application & app)
{
}

void PlaneCollider::FixedUpdate(Application & app)
{
}


PlaneCollider::~PlaneCollider()
{
}
