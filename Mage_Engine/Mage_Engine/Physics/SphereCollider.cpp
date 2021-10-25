#include "SphereCollider.h"



SphereCollider::SphereCollider(Entity &entity) :
	Component(entity),
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


SphereCollider::~SphereCollider()
{
}
