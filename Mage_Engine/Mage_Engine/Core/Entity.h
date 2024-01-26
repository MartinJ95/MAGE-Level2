#pragma once
#include <vector>
#include "Component.h"
#include "Graphics/Camera.h"
#include "Transform.h"
#include "Physics/RigidBody.h"
#include "Physics/PlaneCollider.h"
#include "Physics/SphereCollider.h"
#include "Physics/BoxCollider.h"
#include "Graphics/PointLight.h"
#include "Graphics/SpotLight.h"
#include "Controller.h"
#include "Terrain.h"
#include "Network/RemoteClient.h"
#include "Maths/Matrix.h"
#include "Maths/Vector.h"
#include "Maths/Quaternion.h"
#include "glm/glm.hpp"
#include <map>

class Application;

enum colliderTypes
{
	eNone,
	eSphere,
	ePlane,
	eBox
};

enum inputResult
{
	eNoInput,
	ePending,
	eResolved
};
/*
typedef bool(*componentManager)(Component*, World*, int, collisionData*);

template<typename T> bool manageComponents(Component *component, World *world, int functionNumber, collisionData *data)
{
	switch(functionNumber)
	{
	case 0:
		return Entity::updateComponent<T>(component, *world);
	case 1:
		return Entity::FixedUpdateComponent<T>(component, *world);
	case 2:
		return Entity::CleanUpComponent<T>(component);
	case 3:
		return Entity::ComponentCollision<T>(component, *world, *data);
	default:
		return false;
	}
}

static componentManager const componentManagerTypes[] =
{ manageComponents<Transform>,
manageComponents<Camera>,
manageComponents<Mesh>,
manageComponents<RigidBody>,
manageComponents<PlaneCollider>,
manageComponents<SphereCollider>,
manageComponents<BoxCollider>,
manageComponents<PointLight>,
manageComponents<SpotLight>
};*/

class Entity
{
public:
	Entity(bool active);
	Entity(bool active, Entity &parent);
	Entity(Entity& other);
	Entity(Entity&& other);
	friend class Component;
	template<typename T> T* getComponent()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			if (dynamic_cast<T*>(m_components[i].get()) != NULL)
			{
				return static_cast<T*>(m_components[i].get());
			}
		}
		return nullptr;
	}
	template<typename T> std::vector<T*> getComponents()
	{
		std::vector<std::shared_ptr<T>> componentVector;
		for (int i = 0; i < m_components.size(); i++)
		{
			if (dynamic_cast<T*>(m_components[i].get()) != NULL)
			{
				componentVector.emplace_back(static_cast<T*>(m_components[i].get()));
			}
		}
		return componentVector;
	}
	template<typename T> void addComponent()
	{
		if (std::is_base_of<Component, T>::value == true)
		{
			//std::shared_ptr<T> newComponent = std::make_shared<T>(*this);
			m_components.emplace_back(std::make_shared<T>(*this));
		}
	}
	/*template<typename T> static bool updateComponent(Component *component, World &world)
	{
		if (dynamic_cast<T*>(component) != NULL)
		{
			T *c = static_cast<T*>(component);
			c->Update(world);
			return true;
		}
		return false;
	}
	template<typename T> static bool FixedUpdateComponent(Component *component, World &world)
	{
		if (dynamic_cast<T*>(component) != NULL)
		{
			T *c = static_cast<T*>(component);
			c->FixedUpdate(world);
			return true;
		}
		return false;
	}
	template<typename T> static bool CleanUpComponent(Component *component)
	{
		if (dynamic_cast<T*>(component) != NULL)
		{
			T *c = static_cast<T*>(component);
			delete(c);
			return true;
		}
		return false;
	}
	template<typename T> static bool ComponentCollision(Component *component, World &world, collisionData &data)
	{
		if (dynamic_cast<T*>(component) != NULL)
		{
			T *c = static_cast<T*>(component);
			c->onCollisionEnter(world, data);
			return true;
		}
		return false;
	}*/
	void OnStart(Application &app);
	void Update(Application &app);
	void fixedUpdate(Application &app);
	void OnRender(Application &app) const;
	void OnGUI(Application &app);
	void OnSave(const Application &app, std::ofstream &stream);
	void OnLoad(Application &app, std::ifstream &stream);
	void onCollisionEnter(Application &app, collisionData &data);
	void OnPhysicsStep(Application& app);
	void OnFrameEnd(Application& app);
	Entity* Cleanup(Application& app);
	void createChild(bool active);
	void DeleteComponent(std::weak_ptr<Component> c);
	void operator=(const Entity& rhs)
	{
		m_active = rhs.m_active;
		m_parent = rhs.m_parent;
		if(rhs.m_children.size() > 0)
			m_children = rhs.m_children;
		m_name = rhs.m_name;
		if(rhs.m_components.size() > 0)
			m_components = rhs.m_components;
		for (auto& e : m_children)
		{
			e.m_parent = this;
		}
	}
	void operator=(Entity&& rhs);
	colliderTypes getCollider();
	glm::mat4 getTransformMatrix2D(Application &app);
	glm::mat4 getTransformMatrix3D(Application &app);
	~Entity();
	Entity *m_parent;
	std::vector<Entity> m_children;
	std::string m_name;
	bool m_active;
	bool m_markedForDeletion;
private:
	std::vector<std::shared_ptr<Component>> m_components;
};


typedef void(Entity::* ComponentAddition)();

static std::map<unsigned int, ComponentAddition> componentsAddable{
	std::pair<unsigned int, ComponentAddition>(0, &Entity::addComponent<Camera>),
	std::pair<unsigned int, ComponentAddition>(1, &Entity::addComponent<RigidBody>),
	std::pair<unsigned int, ComponentAddition>(2, &Entity::addComponent<Transform>),
	std::pair<unsigned int, ComponentAddition>(3, &Entity::addComponent<Mesh>),
	std::pair<unsigned int, ComponentAddition>(4, &Entity::addComponent<PointLight>),
	std::pair<unsigned int, ComponentAddition>(5, &Entity::addComponent<SpotLight>),
	std::pair<unsigned int, ComponentAddition>(6, &Entity::addComponent<PlaneCollider>),
	std::pair<unsigned int, ComponentAddition>(7, &Entity::addComponent<BoxCollider>),
	std::pair<unsigned int, ComponentAddition>(8, &Entity::addComponent<SphereCollider>),
	std::pair<unsigned int, ComponentAddition>(9, &Entity::addComponent<Camera>),
	std::pair<unsigned int, ComponentAddition>(10, &Entity::addComponent<Terrain>)
};


