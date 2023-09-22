#include "Entity.h"
#include "Core/Application.h"


Entity::Entity(bool active) :
	m_active(active),
	m_parent(nullptr),
	m_children(),
	m_components()
{
	m_children.reserve(100);
}

Entity::Entity(bool active, Entity &parent) :
	m_active(active),
	m_parent(&parent),
	m_children(),
	m_components()
{
	m_children.reserve(100);
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
		if (m_components[i]->compType != ComponentType::eDefaultComponent) { continue; }
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

void Entity::OnRender(Application & app) const
{
	for (auto &c : m_components)
	{
		if (c->compType != ComponentType::eGraphicsComponent) { continue; }
		c->OnRender(app);
	}
}

void Entity::OnGUI(Application & app)
{
	app.m_viz->GUIEditText("Entity Name", m_name);
	app.m_viz->GUICheckbox("Active", m_active);
	app.m_viz->GUIText("Children");
	int GUIIDIteration = 0;
	for (auto c : m_children)
	{
		GUIIDIteration++;
		app.m_viz->GUIText(c.m_name + "##" + std::to_string(GUIIDIteration));
	}
	GUIIDIteration = 0;
	if (app.m_viz->GUIButton("Add Child")) { createChild(true); }
	for (auto c : m_components)
	{
		GUIIDIteration++;
		c->OnGUI(app);
		if (app.m_viz->GUIButton("Remove Component" + std::string("##") + std::to_string(GUIIDIteration)))
		{
			DeleteComponent(c);
			break;
		}
	}
	GUIIDIteration = 0;
	/*if (app.m_viz->GUIButton("Remove Entity"))
	{
		if (m_parent != nullptr)
		{
			for (std::vector<Entity>::iterator it = m_parent->m_children.begin(); it != m_parent->m_children.end(); it++)
			{
				if (&*it == this)
				{
					m_parent->m_children.erase(it);
					break;
				}
			}
		}
		else
		{
			for (std::vector<Entity*>::iterator it = app.m_currentLevel->m_entities.begin(); it != app.m_currentLevel->m_entities.end(); it++)
			{
				if (*it == this)
				{
					delete *it;
					app.m_currentLevel->m_entities.erase(it);
					break;
				}
			}
		}
	}*/
	//if (app.m_viz->GUIButton("Add Component")) {}
}

void Entity::OnSave(const Application &app, std::ofstream &stream)
{
	stream << "entity" << "\n";
	stream << m_name << "\n";
	for (int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->OnSave(app, stream);
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i].OnSave(app, stream);
	}
	stream << "end" << "\n";
}

void Entity::OnLoad(Application &app, std::ifstream &stream)
{
	stream >> m_name;
	std::string command;
	stream >> command;
	while (true)
	{
		if (command == "end") { return; }
		else if (command == "component")
		{
			unsigned int componentID;
			stream >> componentID;
			if (componentsAddable.find(componentID) != componentsAddable.end())
			{
				ComponentAddition func = componentsAddable.find(componentID)->second;
				(this->*func)();
				m_components.back()->OnLoad(app, stream);
			}
		}
		stream >> command;
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

void Entity::OnPhysicsStep(Application& app)
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
		if (m_components[i]->compType != ComponentType::ePhysicsComponent) { continue; }
		m_components[i]->Update(app);
	}
}

void Entity::createChild(bool active)
{
	Entity newChild(active, *this);
	m_children.push_back(newChild);
}

void Entity::DeleteComponent(Component * c)
{
	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); it++)
	{
		if (*it == c)
		{
			delete c;
			m_components.erase(it);
			break;
		}
	}
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
