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
	app.m_viz->GUIEditInt("size x", m_size.first);
	app.m_viz->GUIEditInt("size.y", m_size.second);
	app.m_viz->GUIEditFloat("tileSize.x", m_tileSize.first);
	app.m_viz->GUIEditFloat("tileSize.Y", m_tileSize.second);
	app.m_viz->GUIEditFloat("randomSeed", m_randomSeed);
	if (app.m_viz->GUIButton("generate mesh"))
	{
		m_isToGenerate = true;
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

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (int i = 0; i < m_size.second; i++)
	{
		for (int j = 0; j < m_size.first; j++)
		{
			vertices.emplace_back();
			vertices.back().position = Mage::Maths::Vector3(
				((m_size.first * (j / m_size.first)) - (m_size.first / 2)) * m_tileSize.first,
				rand() % 100 - 50,
				((m_size.second * (i / m_size.second)) - (m_size.second / 2)) * m_tileSize.second
			);
			vertices.back().normal = Mage::Maths::Vector3(0.f, 1.f, 0.f);
			vertices.back().color = Mage::Maths::Vector3(0.f, 1.f, 0.f);
			vertices.back().texCoords = Mage::Maths::Vector2(
				1 * (j / m_size.first),
				1 * (i / m_size.second)
			);

			if (i < m_size.second - 1 && j < m_size.first - 1)
			{
				indices.emplace_back(j + (i * m_size.second));
				indices.emplace_back((j + 1) + (i * m_size.second));
				indices.emplace_back(j + ((i + 1) * m_size.second));

				indices.emplace_back((j + 1) + (i * m_size.second));
				indices.emplace_back((j + 1) + ((i + 1) * m_size.second));
				indices.emplace_back(j + ((i + 1) * m_size.second));
			}
		}
	}

	app.m_viz->degenerateMesh(m_gennedMeshName);
	app.m_viz->generateMesh(vertices, indices, m_gennedMeshName);
}
