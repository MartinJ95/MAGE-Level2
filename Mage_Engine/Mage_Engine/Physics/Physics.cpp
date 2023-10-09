#include "Physics.h"
#include "Core/Entity.h"
#include "Core/Application.h"
#include "Physics/RigidBody.h"
#include "Core/Entity.h"


Physics::Physics() :
	m_gravity(0.009),
	m_velocityDropoff(0.98)
{
}

void Physics::applyForces(const Application & app, RigidBody & body)
{
	body.m_velocity += (body.m_force + body.m_impulseForce + app.m_worldUp * -m_gravity) / body.m_mass;
	body.m_impulseForce = Mage::Maths::Vector3(0, 0, 0);
	body.m_velocity *= m_velocityDropoff;
	body.m_entity->getComponent<Transform>()->m_position += body.m_velocity;
}

void Physics::handleCollisions(Entity & entity, Application & app)
{
	if (entity.getCollider() != colliderTypes::eNone)
	{
		collisionData data = detectCollision(entity, app);
		if (data.m_hasCollided == true)
		{
			entity.onCollisionEnter(app, data);
			if (entity.getComponent<RigidBody>() != NULL)
			{
				collisionResponse(*entity.getComponent<RigidBody>(), data);
			}
		}
	}
}

collisionData Physics::detectCollision(Entity & entity, Application &app)
{
	colliderTypes collider1 = entity.getCollider();
	std::vector<Entity*> &entities = app.m_currentLevel->m_data.m_entities;
	collisionData data(false, 0, Mage::Maths::Vector3(0,0,0));
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->getCollider() != colliderTypes::eNone && entities[i] != &entity && entities[i]->m_active == true)
		{
			colliderTypes collider2 = app.m_currentLevel->m_data.m_entities[i]->getCollider();
			if (collider2 != colliderTypes::eNone)
			{
				if (collider1 == colliderTypes::eSphere && collider2 == colliderTypes::eSphere)
				{
					data = detectCollisions(*entity.getComponent<SphereCollider>(), *entities[i]->getComponent<SphereCollider>());
					if (data.m_hasCollided == true)
					{
						return data;
					}
				}
				else if (collider1 == colliderTypes::eSphere && collider2 == colliderTypes::eBox)
				{
					data = detectCollisions(*entity.getComponent<SphereCollider>(), *entities[i]->getComponent<BoxCollider>(), false);
					if (data.m_hasCollided == true)
					{
						return data;
					}
				}
				else if (collider1 == colliderTypes::eSphere && collider2 == colliderTypes::ePlane)
				{
					data = detectCollisions(*entity.getComponent<SphereCollider>(), *entities[i]->getComponent<PlaneCollider>());
					if (data.m_hasCollided == true)
					{
						return data;
					}
				}
				else if (collider1 == colliderTypes::eBox && collider2 == colliderTypes::eSphere)
				{
					data = detectCollisions(*entities[i]->getComponent<SphereCollider>(), *entity.getComponent<BoxCollider>(), true);
					if (data.m_hasCollided == true)
					{
						return data;
					}
				}
				else if (collider1 == colliderTypes::eBox && collider2 == colliderTypes::ePlane)
				{
					data = detectCollisions(*entity.getComponent<BoxCollider>(), *entities[i]->getComponent<PlaneCollider>());
					if (data.m_hasCollided == true)
					{
						return data;
					}
				}
				else if (collider1 == colliderTypes::eBox && collider2 == colliderTypes::eBox)
				{
					data = detectCollisions(*entity.getComponent<BoxCollider>(), *entities[i]->getComponent<BoxCollider>());
					if (data.m_hasCollided == true)
					{
						return data;
					}
				}
				else if (collider1 == colliderTypes::ePlane && collider2 == colliderTypes::eSphere)
				{

				}
			}
		}
	}
	return collisionData(false, 0, Mage::Maths::Vector3(0, 0, 0));
}

collisionData Physics::detectCollisions(SphereCollider &collider1, SphereCollider &collider2)
{
	Mage::Maths::Vector3 vectorBetween = ((collider1.m_entity->getComponent<Transform>()->m_position + collider1.m_center) - (collider2.m_entity->getComponent<Transform>()->m_position + collider2.m_center));
	Mage::Maths::Vector3 collisionNormal = Mage::Maths::Vector3(0, 0, 0);
	if (vectorBetween.Length() < collider1.m_radius + collider2.m_radius)
	{
		collisionNormal = (collider1.m_entity->getComponent<Transform>()->m_position - collider2.m_entity->getComponent<Transform>()->m_position).Normalized();
		return collisionData(true, (collider1.m_radius + collider2.m_radius) - vectorBetween.Length(), collisionNormal);
	}
	else
	{
		return collisionData(false, 0, collisionNormal);
	}
}

collisionData Physics::detectCollisions(SphereCollider &collider1, PlaneCollider &collider2)
{
	Mage::Maths::Vector3 vectorBetween = ((collider1.m_entity->getComponent<Transform>()->m_position + collider1.m_center) - (collider2.m_entity->getComponent<Transform>()->m_position + collider2.m_position));
	Mage::Maths::Vector3 collisionNormal = collider2.m_normal;
	if (vectorBetween.Dot(collisionNormal) < collider1.m_radius)
	{
		float penetrationDepth = collider1.m_radius - vectorBetween.Dot(collisionNormal);
		if (penetrationDepth < 0)
		{
			penetrationDepth = -penetrationDepth;
		}
		return collisionData(true, penetrationDepth, collisionNormal);
	}
	else
	{
		return collisionData(false, 0, Mage::Maths::Vector3(0, 0, 0));
	}
}

void Physics::detectClosestEdgeBox(int &closestEdgeIndex, float &penetrationDepth, Mage::Maths::Vector3 &collisionPoint, BoxCollider &collidedBox)
{
	float closestEdge = 0;
	for (int i = 0; i < 6; i++)
	{
		if (compass[i].Dot(collisionPoint.Normalized()) > closestEdge)
		{
			closestEdge = compass[i].Dot(collisionPoint.Normalized());
			closestEdgeIndex = i;
		}
	}
	Mage::Maths::Vector3 edgePoint;
	Mage::Maths::Vector3 vectorBetweenEdgePoint;
	if (closestEdgeIndex < 3)
	{
		edgePoint = (compass[closestEdgeIndex] * collidedBox.m_maxDimensions);
	}
	else
	{
		edgePoint = ((compass[closestEdgeIndex] * -1) * collidedBox.m_minDimensions);
	}
	vectorBetweenEdgePoint = collisionPoint - edgePoint;
	if (closestEdgeIndex < 3)
	{
		penetrationDepth = (compass[closestEdgeIndex] * -1).Dot(vectorBetweenEdgePoint);
	}
	else
	{
		penetrationDepth = (compass[closestEdgeIndex] * -1).Dot(vectorBetweenEdgePoint);
	}
}

collisionData Physics::detectCollisions(SphereCollider & collider1, BoxCollider & collider2, bool inverted)
{
	std::shared_ptr<Transform> t1 = std::shared_ptr<Transform>(collider1.m_entity->getComponent<Transform>());
	std::shared_ptr<Transform> t2 = std::shared_ptr<Transform>(collider2.m_entity->getComponent<Transform>());

	Mage::Maths::Vector3 &minDimensions = collider2.m_minDimensions;
	Mage::Maths::Vector3 &maxDimensions = collider2.m_maxDimensions;

	Mage::Maths::Vector3 facingVector = (t2->m_position - t1->m_position).Normalized();
	Mage::Maths::Vector3 closestPoint = (t1->m_position + (facingVector * collider1.m_radius)) - t2->m_position;
	if (closestPoint.x > minDimensions.x && closestPoint.x < maxDimensions.x)
	{
		if (closestPoint.y > minDimensions.y && closestPoint.y < maxDimensions.y)
		{
			if (closestPoint.z > minDimensions.z && closestPoint.z < maxDimensions.z)
			{
				/*float closestEdge = 0;
				int closestEdgeIndex = 0;
				for (int i = 0; i < 6; i++)
				{
					if (compass[i].dotProduct(closestPoint.normalised()) > closestEdge)
					{
						closestEdge = compass[i].dotProduct(closestPoint.normalised());
						closestEdgeIndex = i;
					}
				}
				float penetrationDepth = 0;
				Mage::Maths::Vector3 edgePoint;
				Mage::Maths::Vector3 vectorBetweenEdgePoint;
				if (closestEdgeIndex < 3)
				{
					edgePoint = (compass[closestEdgeIndex] * collider2.m_maxDimensions);
				}
				else
				{
					edgePoint = ((compass[closestEdgeIndex] * -1) * collider2.m_minDimensions);
				}
				vectorBetweenEdgePoint = closestPoint - edgePoint;
				if (closestEdgeIndex < 3)
				{
					penetrationDepth = (compass[closestEdgeIndex] * -1).dotProduct(vectorBetweenEdgePoint);
				}
				else
				{
					penetrationDepth = (compass[closestEdgeIndex] * -1).dotProduct(vectorBetweenEdgePoint);
				}*/
				int closestEdgeIndex;
				float penetrationDepth;
				detectClosestEdgeBox(closestEdgeIndex, penetrationDepth, closestPoint, collider2);
				return collisionData(true, penetrationDepth, inverted ? (t2->m_position - t1->m_position).Normalized() : compass[closestEdgeIndex], &*collider2.m_entity);
			}
			return collisionData(false, 0, Mage::Maths::Vector3(0, 0, 0));
		}
		return collisionData(false, 0, Mage::Maths::Vector3(0, 0, 0));
	}
	return collisionData(false, 0, Mage::Maths::Vector3(0, 0, 0));
}

collisionData Physics::detectCollisions(BoxCollider &collider1, BoxCollider &collider2)
{
	//detection code
	/*
	collision occurs if: max value on an axis of collider2.min and collider1.max equals more than collider2.min.
	aswell as min value of collider2.max and collider1.min equals less than collider2.max
	once this is true in all dimensions a collision has occured
	*/
	
	Mage::Maths::Vector3 vectorBetween = collider1.m_entity->getComponent<Transform>()->m_position - collider2.m_entity->getComponent<Transform>()->m_position;
	Mage::Maths::Vector3 minDimensions = collider1.m_minDimensions + vectorBetween;
	Mage::Maths::Vector3 maxDimensions = collider1.m_maxDimensions + vectorBetween;
	Mage::Maths::Vector3 min(
		std::fminf(collider2.m_maxDimensions.x, minDimensions.x),
		std::fminf(collider2.m_maxDimensions.y, minDimensions.y),
		std::fminf(collider2.m_maxDimensions.z, minDimensions.z));
	Mage::Maths::Vector3 max(
		std::fmaxf(collider2.m_minDimensions.x, maxDimensions.x),
		std::fmaxf(collider2.m_minDimensions.y, maxDimensions.y),
		std::fmaxf(collider2.m_minDimensions.z, maxDimensions.z));
	if (max > collider2.m_minDimensions && min < collider2.m_maxDimensions)
	{
		//collision true
		//for each axis
		//if on positive side of axis use max of min dimensions
		//if on negative side of axis use min of max dimensions
		/*Mage::Maths::Vector3 collisionPoint(
			(vectorBetween.x >=0.f ) ? std::fmaxf(minDimensions.x, collider2.m_minDimensions.x) : std::fminf(maxDimensions.x, collider2.m_maxDimensions.x),
			(vectorBetween.y >=0.f ) ? std::fmaxf(minDimensions.y, collider2.m_minDimensions.y) : std::fminf(maxDimensions.y, collider2.m_maxDimensions.y),
			(vectorBetween.z >=0.f ) ? std::fmaxf(minDimensions.z, collider2.m_minDimensions.z) : std::fminf(maxDimensions.z, collider2.m_maxDimensions.z));*/
		Mage::Maths::Vector3 minCollisionPoint(
			std::fmaxf(minDimensions.x, collider2.m_minDimensions.x),
			std::fmaxf(minDimensions.y, collider2.m_minDimensions.y),
			std::fmaxf(minDimensions.z, collider2.m_minDimensions.z));
		Mage::Maths::Vector3 maxCollisionPoint(
			std::fminf(maxDimensions.x, collider2.m_maxDimensions.x),
			std::fminf(maxDimensions.y, collider2.m_maxDimensions.y),
			std::fminf(maxDimensions.z, collider2.m_maxDimensions.z));
		Mage::Maths::Vector3 collisionPoint = minCollisionPoint + ((maxCollisionPoint - minCollisionPoint) / 2);
		//maxCollisionPoint = maxCollisionPoint - collider2.m_entity.getComponent<Transform>()->m_position;
		Mage::Maths::Vector3 vectorBetweenCollisionPoint = collisionPoint - collider2.m_entity->getComponent<Transform>()->m_position;
		//std::cout << "x: " << std::to_string(vectorBetweenCollisionPoint.x) << " y: " << std::to_string(vectorBetweenCollisionPoint.y) << " z: " << std::to_string(vectorBetweenCollisionPoint.z) << std::endl;
		int closestEdgeIndex;
		float penetrationDepth;
		detectClosestEdgeBox(closestEdgeIndex, penetrationDepth, vectorBetween, collider2);
		//std::cout << "x: " << std::to_string(compass[closestEdgeIndex].x) << " y: " << std::to_string(compass[closestEdgeIndex].y) << " z: " << std::to_string(compass[closestEdgeIndex].z) << std::endl;
		//std::cout << std::to_string(penetrationDepth) << std::endl;
		if (compass[closestEdgeIndex].x != 0)
		{
			penetrationDepth = (vectorBetween.x >= 0.f) ? (compass[closestEdgeIndex]).Dot(collider2.m_maxDimensions - minDimensions) :
				(
					compass[closestEdgeIndex] * -1).Dot(maxDimensions - collider2.m_minDimensions);
		}
		else if (compass[closestEdgeIndex].y != 0)
		{
			penetrationDepth = (vectorBetween.y >= 0.f) ? (compass[closestEdgeIndex]).Dot(collider2.m_maxDimensions - minDimensions) :
				(compass[closestEdgeIndex] * -1).Dot(maxDimensions - collider2.m_minDimensions);
		}

		else if (compass[closestEdgeIndex].z != 0)
		{
			penetrationDepth = (vectorBetween.z >= 0.f) ? (compass[closestEdgeIndex]).Dot(collider2.m_maxDimensions - minDimensions) :
				(compass[closestEdgeIndex] * -1).Dot(maxDimensions - collider2.m_minDimensions);
		}

		//std::cout << std::to_string(penetrationDepth) << std::endl;
		return collisionData(true, (penetrationDepth >= 0) ? penetrationDepth : -penetrationDepth, compass[closestEdgeIndex], &*collider2.m_entity);
	}
	return collisionData(false, 0, Mage::Maths::Vector3(0, 0, 0));
}

collisionData Physics::detectCollisions(BoxCollider & collider1, PlaneCollider & collider2)
{
	/*Mage::Maths::Vector3 vectorBetween = collider1.m_entity.getComponent<Transform>()->m_position - collider2.m_entity.getComponent<Transform>()->m_position;
	float distance = collider2.m_normal.dotProduct(vectorBetween);
	Mage::Maths::Vector3 vec1, vec2;
	vec1 = Mage::Maths::Vector3(
		(collider2.m_normal.x >= 0) ? collider1.m_minDimensions.x : collider1.m_maxDimensions.x,
		(collider2.m_normal.y >= 0) ? collider1.m_minDimensions.y : collider1.m_maxDimensions.y,
		(collider2.m_normal.z >= 0) ? collider1.m_minDimensions.z : collider1.m_maxDimensions.z);
	vec2 = Mage::Maths::Vector3(
		(collider2.m_normal.x >= 0) ? collider1.m_maxDimensions.x : collider1.m_minDimensions.x,
		(collider2.m_normal.y >= 0) ? collider1.m_maxDimensions.y : collider1.m_minDimensions.y,
		(collider2.m_normal.z >= 0) ? collider1.m_maxDimensions.z : collider1.m_minDimensions.z);
	float positiveSide = (collider2.m_normal.x * vec2.x) + (collider2.m_normal.y * vec2.y) + (collider2.m_normal.z * vec2.z) + distance;
	if (positiveSide > 0)
	{
		return collisionData(false, 0, Mage::Maths::Vector3(0, 0, 0));
	}
	float negativeSide = (collider2.m_normal.x * vec1.x) + (collider2.m_normal.y * vec1.y) + (collider2.m_normal.z * vec1.z) + distance;
	if (negativeSide < 0)
	{
		return collisionData(false, 0, Mage::Maths::Vector3(0, 0, 0));
	}*/
	Entity e(true);
	e.addComponent<Transform>();
	SphereCollider sphere(e);
	Mage::Maths::Vector3 extense = (collider1.m_maxDimensions - collider1.m_minDimensions) / 2.f;
	sphere.m_center = (collider1.m_entity->getComponent<Transform>()->m_position + collider1.m_minDimensions) + extense;
	sphere.m_radius = abs(collider2.m_normal.x*extense.x) + abs(collider2.m_normal.y*extense.y) + abs(collider2.m_normal.z*extense.z);
	return detectCollisions(sphere, collider2);
}

void Physics::collisionResponse(RigidBody & object1, RigidBody & object2, collisionData & collisionData)
{
}

void Physics::collisionResponse(RigidBody & object, collisionData & data)
{
	std::shared_ptr<Transform> t = std::shared_ptr<Transform>(object.m_entity->getComponent<Transform>());
	t->m_position = t->m_position + (data.m_collisionNormal * data.m_penetrationDepth);
	object.m_velocity = object.m_velocity.Reflect(data.m_collisionNormal) * object.m_restitution;
	//temp
	//object.m_force = object.m_force.reflect(data.m_collisionNormal);
}


Physics::~Physics()
{
}
