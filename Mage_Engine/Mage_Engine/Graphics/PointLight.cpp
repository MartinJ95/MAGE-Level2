#include "PointLight.h"


PointLight::PointLight(Entity & entity) :
	Component(entity),
	m_intensity(),
	m_position(),
	m_radius()
{
}

PointLight::~PointLight()
{
}
