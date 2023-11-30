#include "SpotLight.h"
#include "Core/Application.h"


SpotLight::SpotLight(Entity &entity) :
	Component(&entity, 5, ComponentType::eGraphicsComponent),
	m_intensity(),
	m_position(),
	m_fieldOfView(),
	m_range()
{
}

void SpotLight::OnGUI(Application & app)
{
	app.m_viz->GetGUI().GUIText("Spot Light");
	app.m_viz->GetGUI().GUIEditFloat("Field Of View", m_fieldOfView);
	app.m_viz->GetGUI().GUIEditFloat("Range", m_range);
}

void SpotLight::OnSave(const Application& app, std::ofstream& stream) const
{
	m_intensity.Save(stream);
	m_position.Save(stream);
	stream << m_fieldOfView << "\n";
	stream << m_range << "\n";
}

void SpotLight::OnLoad(Application& app, std::ifstream& stream)
{
	m_intensity.Load(stream);
	m_position.Load(stream);
	stream >> m_fieldOfView;
	stream >> m_range;
}


SpotLight::~SpotLight()
{
}
