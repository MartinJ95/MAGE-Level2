#include "Core/Application.h"
#include "Camera.h"

Camera::Camera(Entity &entity) :
	Component(&entity, 9, ComponentType::eGraphicsComponent),
	m_fieldOfView(90)
{
}

void Camera::Update(Application &app)
{

}

void Camera::FixedUpdate(Application &app)
{

}

void Camera::OnGUI(Application & app)
{
	app.m_viz->GUIText("Camera");
	app.m_viz->GUIEditFloat("field of view", m_fieldOfView);
	if (app.m_viz->GUIButton("set as main cam")) { app.m_currentLevel->m_data.m_mainCamera = std::shared_ptr<Camera>(this); }
}

void Camera::OnSave(const Application& app, std::ofstream& stream) const
{
	Component::OnSave(app, stream);
	stream << m_fieldOfView;
}

void Camera::OnLoad(Application& app, std::ifstream& stream)
{
	stream >> m_fieldOfView;
}

Camera::~Camera()
{
}
