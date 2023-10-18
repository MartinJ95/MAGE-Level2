#include "Entity.h"
#include "Core/Application.h"
#include "Graphics/Camera.h"

Entity::Entity(bool active) :
	m_active(active),
	m_parent(nullptr),
	m_children(),
	m_components(),
	m_name(""),
	m_markedForDeletion(false)
{
	m_children.reserve(10);
}

Entity::Entity(bool active, Entity &parent) :
	m_active(active),
	m_parent(&parent),
	m_children(),
	m_components(),
	m_name(""),
	m_markedForDeletion(false)
{
	m_children.reserve(10);
}

Entity::Entity(Entity& other) :
	m_active(other.m_active),
	m_parent(other.m_parent),
	m_children(other.m_children),
	m_components(other.m_components),
	m_name(other.m_name),
	m_markedForDeletion(false)
{	
	std::cout << "copy" << std::endl;
	for (int i = 0; i < other.m_components.size(); i++)
	{
		auto& c = other.m_components[i];
		if (c->compType == ComponentType::eGraphicsComponent)
		{
			//todo
			// when the service locator is running update the level data pointers to correct new component memory location

			bool endReached = false;
			Application* app = ServiceLocator::GetMainService();
			std::vector<Camera*>::iterator camIterator = app->m_currentLevel->m_data.m_cameras.begin();
			std::vector<PointLight*>::iterator pLightIterator = app->m_currentLevel->m_data.m_pointLights.begin();
			std::vector<SpotLight*>::iterator sLightIterator = app->m_currentLevel->m_data.m_spotLights.begin();
			while (!endReached)
			{
				bool test = false;
				if (camIterator != app->m_currentLevel->m_data.m_cameras.end())
				{
					Camera* cam = *camIterator;
					if (cam == c.get())
					{
						*camIterator = static_cast<Camera*>(m_components[i].get());
						endReached = true;
						continue;
					}
					camIterator++;
					test = true;
				}
				if (pLightIterator != app->m_currentLevel->m_data.m_pointLights.end())
				{
					PointLight* pLight = *pLightIterator;
					if (pLight == c.get())
					{
						*pLightIterator = static_cast<PointLight*>(m_components[i].get());
						endReached = true;
						continue;
					}
					pLightIterator++;
					test = true;
				}
				if (sLightIterator != app->m_currentLevel->m_data.m_spotLights.end())
				{
					SpotLight* sLight = *sLightIterator;
					if (sLight == c.get())
					{
						*sLightIterator = static_cast<SpotLight*>(m_components[i].get());
						endReached = true;
						continue;
					}
					sLightIterator++;
					test = true;
				}
				if (test == false) { break; }
			}
		}
	}
	for (auto& e : m_children)
	{
		e.m_parent = this;
	}
	for (auto& c : m_components)
	{
		c->m_entity = this;
	}
}

Entity::Entity(Entity&& other) :
	m_active(other.m_active),
	m_parent(other.m_parent),
	m_children(other.m_children),
	m_components(other.m_components),
	m_name(other.m_name),
	m_markedForDeletion(false)
{
	std::cout << "move" << std::endl;
	for (auto& c : m_components)
	{
		c.get()->m_entity = this;
	}
	for (int i = 0; i < other.m_components.size(); i++)
	{
		auto& c = other.m_components[i];
		if (c->compType == ComponentType::eGraphicsComponent)
		{
			//todo
			// when the service locator is running update the level data pointers to correct new component memory location

			bool endReached = false;
			Application* app = ServiceLocator::GetMainService();
			std::vector<Camera*>::iterator camIterator = app->m_currentLevel->m_data.m_cameras.begin();
			std::vector<PointLight*>::iterator pLightIterator = app->m_currentLevel->m_data.m_pointLights.begin();
			std::vector<SpotLight*>::iterator sLightIterator = app->m_currentLevel->m_data.m_spotLights.begin();
			while (!endReached)
			{
				bool test = false;
				if (camIterator != app->m_currentLevel->m_data.m_cameras.end())
				{
					Camera* cam = *camIterator;
					if (cam == c.get())
					{
						*camIterator = static_cast<Camera*>(m_components[i].get());
						endReached = true;
						continue;
					}
					camIterator++;
					test = true;
				}
				if (pLightIterator != app->m_currentLevel->m_data.m_pointLights.end())
				{
					PointLight* pLight = *pLightIterator;
					if (pLight == c.get())
					{
						*pLightIterator = static_cast<PointLight*>(m_components[i].get());
						endReached = true;
						continue;
					}
					pLightIterator++;
					test = true;
				}
				if (sLightIterator != app->m_currentLevel->m_data.m_spotLights.end())
				{
					SpotLight* sLight = *sLightIterator;
					if (sLight == c.get())
					{
						*sLightIterator = static_cast<SpotLight*>(m_components[i].get());
						endReached = true;
						continue;
					}
					sLightIterator++;
					test = true;
				}
				if (test == false) { break; }
			}
		}
	}
	for (auto& e : m_children)
	{
		e.m_parent = this;
	}
	for (auto& c : m_components)
	{
		c->m_entity = this;
	}
	other.m_parent = nullptr;
	other.m_children.clear();
	other.m_components.clear();
}

void Entity::operator=(Entity&& rhs)
{
	m_active = rhs.m_active;
	m_parent = rhs.m_parent;
	if (rhs.m_children.size() > 0)
		m_children = std::move(rhs.m_children);
	m_name = rhs.m_name;
	if (rhs.m_components.size() > 0)
		m_components = std::move(rhs.m_components);
	for (auto& c : m_components)
	{
		c.get()->m_entity = this;
	}
	for (int i = 0; i < rhs.m_components.size(); i++)
	{
		auto& c = rhs.m_components[i];
		if (c->compType == ComponentType::eGraphicsComponent)
		{
			//todo
			// when the service locator is running update the level data pointers to correct new component memory location

			bool endReached = false;
			Application* app = ServiceLocator::GetMainService();
			std::vector<Camera*>::iterator camIterator = app->m_currentLevel->m_data.m_cameras.begin();
			std::vector<PointLight*>::iterator pLightIterator = app->m_currentLevel->m_data.m_pointLights.begin();
			std::vector<SpotLight*>::iterator sLightIterator = app->m_currentLevel->m_data.m_spotLights.begin();
			while (!endReached)
			{
				bool test = false;
				if (camIterator != app->m_currentLevel->m_data.m_cameras.end())
				{
					Camera* cam = *camIterator;
					if (cam == c.get())
					{
						*camIterator = static_cast<Camera*>(m_components[i].get());
						endReached = true;
						continue;
					}
					camIterator++;
					test = true;
				}
				if (pLightIterator != app->m_currentLevel->m_data.m_pointLights.end())
				{
					PointLight* pLight = *pLightIterator;
					if (pLight == c.get())
					{
						*pLightIterator = static_cast<PointLight*>(m_components[i].get());
						endReached = true;
						continue;
					}
					pLightIterator++;
					test = true;
				}
				if (sLightIterator != app->m_currentLevel->m_data.m_spotLights.end())
				{
					SpotLight* sLight = *sLightIterator;
					if (sLight == c.get())
					{
						*sLightIterator = static_cast<SpotLight*>(m_components[i].get());
						endReached = true;
						continue;
					}
					sLightIterator++;
					test = true;
				}
				if (test == false) { break; }
			}
		}
	}
	for (auto& e : m_children)
	{
		e.m_parent = this;
	}
	rhs.m_parent = nullptr;
	rhs.m_children.clear();
}

void Entity::OnStart(Application& app)
{
	for (int i = 0; i < m_children.size(); i++) 
	{
		m_children[i].OnStart(app);
	}

	for (int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->OnStart(app);
	}
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
	for (auto& e : m_children)
	{
		e.OnRender(app);
	}
}

void Entity::OnGUI(Application & app)
{
	app.m_viz->GUIEditText("Entity Name", m_name);
	app.m_viz->GUICheckbox("Active", m_active);
	app.m_viz->GUIText("Children");
	int GUIIDIteration = 0;
	for (auto &c : m_children)
	{
		GUIIDIteration++;
		app.m_viz->GUIText(c.m_name + "##" + std::to_string(GUIIDIteration));
	}
	GUIIDIteration = 0;
	if (app.m_viz->GUIButton("Add Child")) { createChild(true); }
	for (auto &c : m_components)
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

void Entity::OnFrameEnd(Application& app)
{
	for (int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->OnFrameEnd(app);
	}
}

Entity* Entity::Cleanup(Application& app)
{
	for (int i = m_components.size()-1; i >= 0; i--)
	{
		m_components[i]->Cleanup();
	}
	/*for (std::vector<Component*>::reverse_iterator it = m_components.rbegin(); it != m_components.rend(); ++it)
	{
		Component* c = *it;
		c->Cleanup();
	}*/

	if (!m_markedForDeletion) { return nullptr; }

	if (m_parent != nullptr)
	{
		for (int i = m_parent->m_children.size(); i > 0; --i)
		{
			if (&m_parent->m_children[i] == this)
			{
				m_parent->m_children.erase(m_parent->m_children.begin() + i);
			}
		}
		/*
		for (std::vector<Entity>::reverse_iterator it = m_parent->m_children.rbegin(); it != m_parent->m_children.rend(); it++)
		{
			if (&*it == this)
			{
				m_parent->m_children.erase(std::next(it).base());
				return nullptr;
			}
		}
		*/
	}

	return this;
}

void Entity::createChild(bool active)
{
	//Entity newChild(active, *this);
	m_children.emplace_back(active, *this);
}

void Entity::DeleteComponent(std::weak_ptr<Component> c)
{
	for (std::vector<std::shared_ptr<Component>>::iterator it = m_components.begin(); it != m_components.end(); it++)
	{
		if (*it == c.lock())
		{
			//delete c;
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

	Transform* t = getComponent<Transform>();
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
	Transform* t = getComponent<Transform>();
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
	/*for (auto c : m_components)
	{
		delete(c);
	}*/
	m_components.clear();
}