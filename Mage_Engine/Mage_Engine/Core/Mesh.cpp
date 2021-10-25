#include "Mesh.h"
#include "Core/Application.h"
#include "Core/Entity.h"
#include "Core/Transform.h"


Mesh::Mesh(Entity &entity) :
	Component(entity),
	m_is3D(true),
	m_meshName(),
	m_textureName(),
	m_shaderName()
{
}

void Mesh::Update(Application &app)
{
	if (!m_is3D)
	{
		app.m_viz->render2D(m_meshName, m_textureName, m_shaderName, m_entity.getTransformMatrix2D(app));
	}
	else
	{
		app.m_viz->render3D(m_meshName, m_textureName, m_shaderName, m_entity.getTransformMatrix3D(app), *app.m_currentLevel->m_mainCamera, app.m_worldUp, app);
	}
}

void Mesh::FixedUpdate(Application &app)
{

}


Mesh::~Mesh()
{
}
