#pragma once
#include "Core/Component.h"
class Camera :
	public Component
{
public:
	Camera(Entity &entity);
	void Update(Application &app) override;
	void FixedUpdate(Application &app) override;
	void OnGUI(Application &app) override;
	virtual void OnSave(const Application& app, std::ofstream& stream) const override;
	virtual void OnLoad(Application& app, std::ifstream& stream) override;
	~Camera();
	float m_fieldOfView;
};

