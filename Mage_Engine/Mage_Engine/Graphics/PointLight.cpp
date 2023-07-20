#include "PointLight.h"
#include "Core/Application.h"


PointLight::PointLight(Entity & entity) :
	Component(entity),
	m_intensity(),
	m_position(),
	m_radius()
{
}

void PointLight::OnGUI(Application & app)
{
	app.m_viz->GUIText("Point Light");
	app.m_viz->GUIVector3("light intensity", m_intensity);
	app.m_viz->GUIVector3("lightPosition", m_position);
	app.m_viz->GUIEditFloat("radius", m_radius);
}

PointLight::~PointLight()
{
}
