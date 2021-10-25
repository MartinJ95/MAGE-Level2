#include "Camera.h"



Camera::Camera(Entity &entity) :
	Component(entity),
	m_fieldOfView(90)
{
}

void Camera::Update(Application &app)
{

}

void Camera::FixedUpdate(Application &app)
{

}

Camera::~Camera()
{
}
