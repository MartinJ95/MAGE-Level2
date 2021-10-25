#include "Entity.h"
#include "Application.h"


Entity::Entity(bool active) :
	m_active(active),
	m_parent(nullptr),
	m_children(),
	m_components()
{
}

Entity::Entity(bool active, Entity &parent) :
	m_active(active),
	m_parent(&parent),
	m_children(),
	m_components()
{
}

void Entity::Update(Application &app)
{
	for (int i = 0; i < m_components.size(); i++)
	{
		/*for (auto f : componentManagerTypes)
		{
			if (f(m_components[i], &world, 0, nullptr) == true)
			{
				break;
			}
		}*/
		m_components[i]->Update(app);
	}
}

void Entity::fixedUpdate(Application &app)
{
	for (int i = 0; i < m_components.size(); i++)
	{
		/*for (auto f : componentManagerTypes)
		{
			if (f(m_components[i], &world, 1, nullptr) == true)
			{
				break;
			}
		}*/
		m_components[i]->FixedUpdate(app);
	}
}

void Entity::onCollisionEnter(Application & app, collisionData & data)
{
	for (int i = 0; i < m_components.size(); i++)
	{
		/*for (auto f : componentManagerTypes)
		{
			if (f(m_components[i], &world, 3, &data) == true)
			{
				break;
			}
		}*/
		m_components[i]->onCollisionEnter(app, data);
	}
}

void Entity::createChild(bool active)
{
	Entity newChild(active, *this);
	m_children.push_back(newChild);
}

colliderTypes Entity::getCollider()
{
	if (getComponent<SphereCollider>() != NULL)
	{
		return colliderTypes::eSphere;
	}
	else if (getComponent<PlaneCollider>() != NULL)
	{
		return colliderTypes::ePlane;
	}
	else if (getComponent<BoxCollider>() != NULL)
	{
		return colliderTypes::eBox;
	}
	else
	{
		return colliderTypes::eNone;
	}
}

glm::mat4 Entity::getTransformMatrix2D(Application &app)
{

	Transform *t = getComponent<Transform>();
	glm::mat4 transformMatrix = glm::mat4(1.f);
	float meshSizeX = 1;
	float meshSizeY = 1;
	Mage::Maths::Vector3 worldPosition = t->worldPosition();
	Mage::Maths::Vector3 worldRotation = t->worldRotation();
	Mage::Maths::Vector3 worldScale = t->worldScale();
	meshSizeX /= app.m_viz->m_screenWidth;
	meshSizeY /= app.m_viz->m_screenHeight;
	transformMatrix = glm::translate(transformMatrix, glm::vec3((((worldPosition.x / app.m_viz->m_screenWidth) * 2) - 1), (((worldPosition.y / app.m_viz->m_screenHeight) * 2) - 1), worldPosition.z));
	transformMatrix = glm::rotate(transformMatrix, glm::radians(worldRotation.x), glm::vec3(1, 0, 0));
	transformMatrix = glm::rotate(transformMatrix, glm::radians(worldRotation.y), glm::vec3(0, 1, 0));
	transformMatrix = glm::rotate(transformMatrix, glm::radians(worldRotation.z), glm::vec3(0, 0, 1));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(meshSizeX *worldScale.x, meshSizeY *worldScale.y, worldScale.y));
	return transformMatrix;
}

glm::mat4 Entity::getTransformMatrix3D(Application &app)
{
	Transform *t = getComponent<Transform>();
	glm::mat4 transformMatrix = glm::mat4(1.f);
	Mage::Maths::Vector3 worldPosition = t->worldPosition();
	Mage::Maths::Vector3 worldRotation = t->worldRotation();
	Mage::Maths::Vector3 worldScale = t->worldScale();
	transformMatrix = glm::translate(transformMatrix, glm::vec3(worldPosition.x, worldPosition.y, worldPosition.z));
	transformMatrix = glm::rotate(transformMatrix, glm::radians(worldRotation.z), glm::vec3(0, 0, 1));
	transformMatrix = glm::rotate(transformMatrix, glm::radians(worldRotation.y), glm::vec3(0, 1, 0));
	transformMatrix = glm::rotate(transformMatrix, glm::radians(worldRotation.x), glm::vec3(1, 0, 0));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(worldScale.x, worldScale.y, worldScale.z));
	return transformMatrix;
}

Entity::~Entity()
{
	/*for (int i = 0; i < m_components.size(); i++)
	{
		for (auto f : componentManagerTypes)
		{
			if (f(m_components[i], nullptr, 2, nullptr) == true)
			{
				break;
			}
		}
		for (int i = 0; i < m_components.size(); i++)
		{
			delete(m_components[i]);
		}
	}*/
	for (auto c : m_components)
	{
		delete(c);
	}
	m_components.clear();
}
