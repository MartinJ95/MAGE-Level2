#pragma once
#include "Component.h"
#include "iostream"

class Entity;
class World;

class Mesh :
	public Component
{
public:
	Mesh(Entity &entity);
	void Update(Application &app) override;
	void FixedUpdate(Application &app) override;
	void OnRender(Application &app) override;
	void OnGUI(Application &app) override;
	virtual void OnSave(const Application& app, std::ofstream& stream) const override;
	virtual void OnLoad(Application& app, std::ifstream& stream) override;
	~Mesh();
	bool m_is3D;
	std::string m_meshName;
	std::string m_textureName;
	std::string m_shaderName;
};

