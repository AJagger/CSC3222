/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class acts as the world - updating positions/velocities, storing spring information and calling collision detection systems
*/

#include "stdafx.h"
#include "PhysicsResolver.h"
#include "DataArray.cpp"
#include "../Game/CSC3222P1/Vec3.h"

PhysicsResolver::PhysicsResolver()
{
}

PhysicsResolver::~PhysicsResolver()
{
}

void PhysicsResolver::SimulateWorld(DataArray<DemoGameObject> *gameObjects, float dt)
{

	dt = dt / 300;

	//Calculate forces for objects connected by springs
	Spring *spring = springs.TryToGetFirst();
	if (spring != nullptr)
	{
		//For all springs in this simulation, the parent (player) object is treated as if it has infinite mass so that only the child moves
		spring->childObject->currentPhysState.actingForce = DetermineSpringForce(spring->parentObject, spring->childObject);

		while (springs.IsNext())
		{
			spring = springs.Next();
			if (spring != nullptr)
			{
				//For all springs in this simulation, the parent (player) object is treated as if it has infinite mass so that only the child moves
				spring->childObject->currentPhysState.actingForce = DetermineSpringForce(spring->parentObject, spring->childObject);
			}
		}
	}


	//Collision detection Goes here
	Collision::CheckCollisions(gameObjects, &collisions);
	Collision::DetectTerrain(gameObjects);

	//Integrate velocity and position
	DemoGameObject *object = gameObjects->TryToGetFirst();
	if (object != nullptr)
	{
		if(object->entityType == PLAYER || object->entityType == DRONE)
		{
			Integrate(object->currentPhysState, dt);
			object->currentPhysState.actingForce = Vec3(0, 0, 0);
		}

		while (gameObjects->IsNext())
		{
			object = gameObjects->Next();
			if (object != nullptr)
			{
				if (object->entityType == PLAYER || object->entityType == DRONE)
				{
					Integrate(object->currentPhysState, dt);
					object->currentPhysState.actingForce = Vec3(0, 0, 0);
				}
			}
		}
	}
}

void PhysicsResolver::AddSpring(DataArray<DemoGameObject>* gameObjects, int parentId, int childId)
{
	DemoGameObject* parent = gameObjects->TryToGet(parentId);
	DemoGameObject* child = gameObjects->TryToGet(childId);

	if(parent != nullptr && child != nullptr)
	{
		Spring *spring = springs.CreateNew();
		spring->parentId = parentId;
		spring->childId = childId;
		spring->parentObject = parent;
		spring->childObject = child;
	}
}

void PhysicsResolver::RemoveSpring(int id)
{
	Spring *spring = springs.TryToGetFirst();
	if (spring != nullptr)
	{
		if(spring->childId == id)
		{
			int springId = springs.GetId(*spring);
			springs.Free(springId);
		}

		while (springs.IsNext())
		{
			spring = springs.Next();
			if (spring != nullptr)
			{
				if (spring->childId == id)
				{
					int springId = springs.GetId(*spring);
					springs.Free(springId);
				}
			}
		}
	}
}

//Semi-Implicit Euler Integration
void PhysicsResolver::Integrate(State &state, float dt)
{
	float maxVelocity = 0.5 * state.velocityModifier;
	state.velocity = state.velocity + (Acceleration(state) * dt);
	if(state.velocity.MagnitudeXY() > maxVelocity)
	{
		float scale = maxVelocity / state.velocity.MagnitudeXY();
		state.velocity = state.velocity*scale;
	}
	state.position = state.position + (state.velocity * dt);
}

Vec3 PhysicsResolver::Acceleration(State &state)
{
	//If force is acting on the object
	if(state.actingForce.x !=  0.0f || state.actingForce.y != 0.0f)
	{
		//a = f/m
		return state.actingForce / state.mass;
	}
	
	//if no force acting on the object, add a small dampening
	return state.velocity * -0.2;
}

Vec3 PhysicsResolver::DetermineSpringForce(DemoGameObject * parentObject, DemoGameObject * childObject)
{
	//used later for an equilibrium position where the two objects are touching but not attempting to merge
	float springRestingLength = parentObject->currentPhysState.radius + childObject->currentPhysState.radius;
	float k = 0.1;
	float b = 1;

	//Vector displacement
	Vec3 x = childObject->currentPhysState.position - parentObject->currentPhysState.position;
	x.z = 0;	//Set the z value to 0 since this is a 2D usage of a 3D vector
	float distance = x.MagnitudeXY();

	//Relative velocity
	Vec3 v = childObject->currentPhysState.velocity - parentObject->currentPhysState.velocity;

	//F = -k(|x|-d)(x/|x|) - bv
	return -k*(distance - springRestingLength)*(x.normalise()) - b*v;
}
