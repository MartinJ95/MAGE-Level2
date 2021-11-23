#include "SpotLight.h"
#include "Core/Application.h"


SpotLight::SpotLight(Entity &entity) :
	Component(entity),
	m_intensity(),
	m_position(),
	m_fieldOfView(),
	m_range()
{
}

void SpotLight::OnGUI(Application & app)
{
	app.m_viz->GUIText("Spot Light");
	app.m_viz->GUIEditFloat("Field Of View", m_fieldOfView);
	app.m_viz->GUIEditFloat("Range", m_range);
}


SpotLight::~SpotLight()
{
}
