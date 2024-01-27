#include "Terrain.h"
#include "Core/Application.h"

Terrain::Terrain(Entity& entity) : Mesh(entity, 10), m_size(), m_tileSize(), m_randomSeed(), m_gennedMeshName()
{
}

void Terrain::OnStart(Application& app)
{
	GenerateMesh(app);
}

void Terrain::OnGUI(Application& app)
{
	Mesh::OnGUI(app);
	app.m_viz->GetGUI().GUIEditInt("size x", m_size.first);
	app.m_viz->GetGUI().GUIEditInt("size.y", m_size.second);
	app.m_viz->GetGUI().GUIEditFloat("tileSize.x", m_tileSize.first);
	app.m_viz->GetGUI().GUIEditFloat("tileSize.Y", m_tileSize.second);
	app.m_viz->GetGUI().GUIEditFloat("randomSeed", m_randomSeed);
	if (app.m_viz->GetGUI().GUIButton("generate mesh"))
	{
		m_isToGenerate = true;
	}
	if (app.m_viz->GetGUI().GUIButton("generate foliage"))
	{
		m_isToGenerateFoliage = true;
	}
}

void Terrain::OnSave(const Application& app, std::ofstream& stream) const
{
	Mesh::OnSave(app, stream);
	stream << m_size.first << "\n";
	stream << m_size.second << "\n";
	stream << m_tileSize.first << "\n";
	stream << m_tileSize.second << "\n";
	stream << m_randomSeed << "\n";
	stream << m_gennedMeshName << "\n";
}

void Terrain::OnLoad(Application& app, std::ifstream& stream)
{
	Mesh::OnLoad(app, stream);
	stream >> m_size.first;
	stream >> m_size.second;
	stream >> m_tileSize.first;
	stream >> m_tileSize.second;
	stream >> m_randomSeed;
	stream >> m_gennedMeshName;
	m_isToGenerate = true;
}

void Terrain::OnFrameEnd(Application& app)
{
	if (m_isToGenerate)
	{
		m_isToGenerate = false;
		GenerateMesh(app);
	}
	if (m_isToGenerateFoliage)
	{
		m_isToGenerateFoliage = false;
		GenerateFoliage(app);
	}
}

//gets a point on the generated terrain with the given points pos[x, y] == [transform.position.x, transform.position.z]
Mage::Maths::Vector3 Terrain::GetPointOnTerrain(const Mage::Maths::Vector2& pos, const Application& app) const
{
	Transform* t = m_entity->getComponent<Transform>();
	MeshGL* mesh = app.m_viz->GetMeshGeneration().GetMesh(m_gennedMeshName);

	//returns zero vector if the genned mesh was not found
	if (mesh == nullptr)
		return Mage::Maths::Vector3(0.f, 0.f, 0.f);

	Mage::Maths::Vector2 scaledPos = pos;

	if (t != nullptr)
		scaledPos *= Mage::Maths::Vector2(t->m_scale.x, t->m_scale.z);

	// the offset from actual pos to index on grid
	std::pair<float, float> offset = std::pair<float, float>(
		static_cast<float>(m_size.first * m_tileSize.first) / 2.f,
		static_cast<float>(m_size.second * m_tileSize.second) / 2.f
		);

	// finding the grid tile index for the scaled pos to be on
	std::pair<int, int> currentTile = std::pair<int, int>(
		static_cast<int>(floorf((scaledPos.x + offset.first)/m_tileSize.first)),
		static_cast<int>(floorf((scaledPos.y + offset.second)/m_tileSize.second))
		);

	// checks to see if the found tile is valid and on the terrain
	if (currentTile.first < 0 || currentTile.first >= m_size.first-1 ||
		currentTile.second < 0 || currentTile.second >= m_size.second-1)
		return Mage::Maths::Vector3(0.f, 0.f, 0.f);

	Mage::Maths::Vector3 vertexPos[4]{
		mesh->m_vertices.at(currentTile.first + m_size.first * currentTile.second).position,
		mesh->m_vertices.at((currentTile.first + 1) + m_size.first * currentTile.second).position,
		mesh->m_vertices.at(currentTile.first + m_size.first * (currentTile.second + 1)).position,
		mesh->m_vertices.at((currentTile.first + 1) + m_size.first * (currentTile.second + 1)).position
	};

	Mage::Maths::Vector3 cornerToPos = Mage::Maths::Vector3(scaledPos.x, 0, scaledPos.y) - (vertexPos[0]*Mage::Maths::Vector3(1.f, 0.f, 1.f));

	float percentagex = cornerToPos.Dot(((vertexPos[1] - vertexPos[0])*Mage::Maths::Vector3(1.f, 0.f, 1.f)).Normalized()) / m_tileSize.first;
	float percentagey = cornerToPos.Dot(((vertexPos[2] - vertexPos[0])*Mage::Maths::Vector3(1.f, 0.f, 1.f)).Normalized()) / m_tileSize.second;

	bool isMoreTopLeft =
		(percentagey
		+
		percentagex) * 0.5f
		< 0.5f ? true : false;

	//completes the forth point in the relevant parrelelogram based on which triangle the position is on
	// nah actually not needed haha maybe
	//Mage::Maths::Vector3 standInVector = (vertexPos[isMoreTopLeft ? 2 : 3] - vertexPos[0]).Reflect((vertexPos[3] - vertexPos[0]).Normalized());

	if (isMoreTopLeft)
	{
		return vertexPos[0] + Mage::Maths::Vector3(
			(vertexPos[1].x - vertexPos[0].x) * percentagex,
			((vertexPos[1].y - vertexPos[0].y) * percentagex) + ((vertexPos[3].y - vertexPos[0].y) * percentagey),
			(vertexPos[3].z - vertexPos[0].z) * percentagey
			);
	}
	return vertexPos[0] + Mage::Maths::Vector3(
		(vertexPos[3].x - vertexPos[0].x) * percentagex,
		((vertexPos[3].y - vertexPos[0].y) * percentagex) + ((vertexPos[2].y - vertexPos[0].y) * percentagey),
		(vertexPos[2].z - vertexPos[0].z) * percentagey
	);
}

Terrain::~Terrain()
{
}

void Terrain::GenerateMesh(Application& app)
{
	if (m_size.first < 2 || m_size.second < 2 || m_tileSize.first == 0.f || m_tileSize.second == 0.f)
	{
		return;
	}

	srand(m_randomSeed);

	m_gennedMeshName = m_meshName;

	//std::vector<Vertex> vertices;
	//std::vector<unsigned int> indices;
	VertexData vData;

	vData.vertices.reserve(m_size.first * m_size.second);

	for (int i = 0; i < m_size.second; i++)
	{
		for (int j = 0; j < m_size.first; j++)
		{
			vData.vertices.emplace_back();
			vData.vertices.back().position = Mage::Maths::Vector3(
				((static_cast<float>(m_size.first) * (static_cast<float>(j) / static_cast<float>(m_size.first))) - (static_cast<float>(m_size.first) / 2.f)) * static_cast<float>(m_tileSize.first),
				(rand() % 10 - 5) * 0.9f,
				((static_cast<float>(m_size.second) * (static_cast<float>(i) / static_cast<float>(m_size.second))) - (static_cast<float>(m_size.second) / 2.f)) * static_cast<float>(m_tileSize.second)
			);
			vData.vertices.back().normal = Mage::Maths::Vector3(0.f, 1.f, 0.f);
			vData.vertices.back().color = Mage::Maths::Vector3(0.f, 1.f, 0.f);
			vData.vertices.back().texCoords = Mage::Maths::Vector2(
				static_cast<float>(m_size.first) * (static_cast<float>(j) / static_cast<float>(m_size.first-1)),
				static_cast<float>(m_size.second) * (static_cast<float>(i) / static_cast<float>(m_size.second-1))
			);

			if (i < m_size.second - 1 && j < m_size.first - 1)
			{
				vData.indices.emplace_back(j + (i * m_size.second));
				vData.indices.emplace_back((j + 1) + (i * m_size.second));
				vData.indices.emplace_back(j + ((i + 1) * m_size.second));
				
				vData.indices.emplace_back((j + 1) + (i * m_size.second));
				vData.indices.emplace_back((j + 1) + ((i + 1) * m_size.second));
				vData.indices.emplace_back(j + ((i + 1) * m_size.second));
			}
		}
	}

	app.m_viz->GetMeshGeneration().DegenerateMesh(m_gennedMeshName);
	app.m_viz->GetMeshGeneration().GenerateMesh(vData, m_gennedMeshName);
}

void Terrain::GenerateFoliage(Application& app)
{
	//finds the offset for the generated positions as the origin point is in center
	std::pair<float, float> offsets = std::pair<float, float>(
		(static_cast<float>(m_size.first) * m_tileSize.first) * 0.5f,
		(static_cast<float>(m_size.second) * m_tileSize.second) * 0.5f
		);
	m_entity->m_children.reserve(m_size.first * m_size.second);
	srand(m_randomSeed);
	for (float i = -offsets.second; i < (m_size.second * m_tileSize.second)-offsets.second; i += (rand() % static_cast<int>(m_tileSize.second) + 10.f))
	{
		for (float j = -offsets.first; j < (m_size.first * m_tileSize.first)-offsets.first; j += (rand() % static_cast<int>(m_tileSize.first) + 10.f))
		{
			Mage::Maths::Vector3 gennedPosition = GetPointOnTerrain(Mage::Maths::Vector2(j, i), app);
			if (gennedPosition == Mage::Maths::Vector3(0.f, 0.f, 0.f))
			{
				continue;
			}

			//gennedPosition += Mage::Maths::Vector3(0.f, 1.f, 0.f);
			m_entity->m_children.emplace_back(true, *m_entity);
			m_entity->m_children.back().addComponent<Transform>();
			Transform* t = m_entity->m_children.back().getComponent<Transform>();
			t->m_scale = Mage::Maths::Vector3(0.25f, rand() % 2 + 2, 0.25f);
			t->m_position = gennedPosition + Mage::Maths::Vector3(0.f, t->m_scale.y/2, 0.f);
			m_entity->m_children.back().addComponent<Mesh>();
			Mesh* m = m_entity->m_children.back().getComponent<Mesh>();
			m->m_is3D = true;
			m->m_meshName = "box";
			m->m_shaderName = "default3DShader";
			m->m_textureName = "wood";

			m_entity->m_children.back().m_children.emplace_back(true, m_entity->m_children.back());
			m_entity->m_children.back().m_children.back().addComponent<Transform>();
			Transform* t2 = m_entity->m_children.back().m_children.back().getComponent<Transform>();
			t2->m_position = Mage::Maths::Vector3(0.f, t->m_scale.y, 0.f);
			m_entity->m_children.back().m_children.back().addComponent<Mesh>();
			Mesh* m2 = m_entity->m_children.back().m_children.back().getComponent<Mesh>();
			m2->m_is3D = true;
			m2->m_meshName = "sphere";
			m2->m_shaderName = "default3DShader";
			m2->m_textureName = "grass";
			t2->m_entity = &m_entity->m_children.back().m_children.back();
			m2->m_entity = &m_entity->m_children.back().m_children.back();

			m_entity->m_children.back().addComponent<PointLight>();
			PointLight* p = m_entity->m_children.back().getComponent<PointLight>();
			p->m_intensity = Mage::Maths::Vector3(0.6f, 0.6f, 0.6f);
			p->m_radius = 20.f;
			p->m_position = gennedPosition + Mage::Maths::Vector3(0.f, 1.f, 0.f);
			app.m_currentLevel->m_data.m_pointLights.emplace_back(m_entity->m_children.back().getComponent<PointLight>());
		}
	}
}
