#pragma once
#include "Maths/Vector.h"

class Entity;
class RigidBody;
class Application;
class SphereCollider;
class PlaneCollider;
class BoxCollider;

enum class collisionTypes
{
	eSphereToSphere,
	eSphereToPlane,
	eSphereToBox
};

struct collisionData
{
	collisionData(const bool &collided, const float &penetrationDepth, const Mage::Maths::Vector3 &collisionNormal, Entity *entity = nullptr) : 
		m_hasCollided(collided), 
		m_penetrationDepth(penetrationDepth), 
		m_collisionNormal(collisionNormal),
	collidedEntity(entity){}
	bool m_hasCollided;
	float m_penetrationDepth;
	Mage::Maths::Vector3 m_collisionNormal;
	Entity *collidedEntity;
};

class Physics
{
public:
	Physics();
	void applyForces(const Application &app, RigidBody &body);
	void handleCollisions(Entity &entity, Application &app);
	collisionData detectCollision(Entity &entity1, Application &app);
	collisionData detectCollisions(SphereCollider &collider1, SphereCollider &collider2);
	collisionData detectCollisions(SphereCollider &collider1, PlaneCollider &collider2);
	void detectClosestEdgeBox(int &closestEdgeIndex, float &penetrationDepth, Mage::Maths::Vector3 &collisionPoint, BoxCollider &collidedBox);
	collisionData detectCollisions(SphereCollider &collider1, BoxCollider &collider2, bool inverted);
	collisionData detectCollisions(BoxCollider &collider1, BoxCollider &collider2);
	collisionData detectCollisions(BoxCollider &collider1, PlaneCollider &collider2);
	void collisionResponse(RigidBody &object1, RigidBody &object2, collisionData &collisionData);
	void collisionResponse(RigidBody &object, collisionData &data);
	~Physics();
	float m_gravity;
	float m_velocityDropoff;
	Mage::Maths::Vector3 compass[6]
	{
		Mage::Maths::Vector3(1, 0, 0),
		Mage::Maths::Vector3(0, 1, 0),
		Mage::Maths::Vector3(0, 0, 1),
		Mage::Maths::Vector3(-1, 0, 0),
		Mage::Maths::Vector3(0, -1, 0),
		Mage::Maths::Vector3(0, 0, -1)
	};
};

