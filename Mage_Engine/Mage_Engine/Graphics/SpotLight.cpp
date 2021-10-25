#include "SpotLight.h"



SpotLight::SpotLight(Entity &entity) :
	Component(entity),
	m_intensity(),
	m_position(),
	m_fieldOfView(),
	m_range()
{
}


SpotLight::~SpotLight()
{
}
