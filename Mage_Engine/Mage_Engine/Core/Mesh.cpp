#include "Mesh.h"
#include "Core/Application.h"
#include "Core/Entity.h"
#include "Core/Transform.h"


Mesh::Mesh(Entity &entity, unsigned int ID) :
	Component(entity, ID, ComponentType::eGraphicsComponent),
	m_is3D(true),
	m_meshName(),
	m_textureName(),
	m_shaderName()
{
}

void Mesh::Update(Application &app)
{
	
}

void Mesh::FixedUpdate(Application &app)
{

}

void Mesh::OnRender(Application & app) const
{
	if (!m_is3D)
	{
		app.m_viz->render2D(m_meshName, m_textureName, m_shaderName, m_entity.getTransformMatrix2D(app));
	}
	else
	{
		app.m_viz->render3D(m_meshName, m_textureName, m_shaderName, m_entity.getTransformMatrix3D(app), *app.m_currentLevel->m_data.m_mainCamera, app.m_worldUp, app);
	}
}

void Mesh::OnGUI(Application & app)
{
	app.m_viz->GUICheckbox("is 3d", m_is3D);
	app.m_viz->GUIEditText("mesh name", m_meshName);
	app.m_viz->GUIEditText("texture name", m_textureName);
	app.m_viz->GUIEditText("shaderName", m_shaderName);
}

void Mesh::OnSave(const Application& app, std::ofstream& stream) const
{
	Component::OnSave(app, stream);
	stream << m_is3D << "\n";
	stream << m_meshName << "\n";
	stream << m_textureName << "\n";
	stream << m_shaderName << "\n";
}

void Mesh::OnLoad(Application& app, std::ifstream& stream)
{
	stream >> m_is3D;
	stream >> m_meshName;
	stream >> m_textureName;
	stream >> m_shaderName;
}


Mesh::~Mesh()
{
}
