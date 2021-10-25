#include "Physics/BoxCollider.h"



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

BoxCollider::~BoxCollider()
{
}
