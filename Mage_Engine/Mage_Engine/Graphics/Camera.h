#pragma once
#include "Core/Component.h"
class Camera :
	public Component
{
public:
	Camera(Entity &entity);
	void Update(Application &app) override;
	void FixedUpdate(Application &app) override;
	~Camera();
	float m_fieldOfView;
};

