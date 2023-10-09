#include "PointLight.h"
#include "Core/Application.h"


PointLight::PointLight(Entity & entity) :
	Component(&entity, 4, ComponentType::eGraphicsComponent),
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

void PointLight::OnSave(const Application& app, std::ofstream& stream) const
{
	Component::OnSave(app, stream);
	m_intensity.Save(stream);
	m_position.Save(stream);
	stream << m_radius << "\n";
}

void PointLight::OnLoad(Application& app, std::ifstream& stream)
{
	m_intensity.Load(stream);
	m_position.Load(stream);
	stream >> m_radius;
}

PointLight::~PointLight()
{
}
