#pragma once
#include "Core/Mesh.h"

class Terrain : public Mesh
{
public:
	Terrain(Entity& entity);
	void OnStart(Application& app) override;
	void OnGUI(Application& app) override;
	virtual void OnSave(const Application& app, std::ofstream& stream) const override;
	virtual void OnLoad(Application& app, std::ifstream& stream) override;
	virtual void OnFrameEnd(Application& app);
	Mage::Maths::Vector3 GetPointOnTerrain(const Mage::Maths::Vector2 &pos, const Application &app) const;
	~Terrain();
private:
	void GenerateMesh(Application& app);
private:
	std::pair<int, int> m_size;
	std::pair<float, float> m_tileSize;
	float m_randomSeed;
	std::string m_gennedMeshName;
	bool m_isToGenerate;
};

