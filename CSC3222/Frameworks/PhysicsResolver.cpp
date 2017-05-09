/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class joins the physics engine Box2D to the rest of the engine. It allows for the creation and processing of game engine objects and data into
* objects and data used by the physics engine and vica versa.
*/

#include "stdafx.h"
#include "PhysicsResolver.h"
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

	//Collision detection Goes here

	DemoGameObject *object = gameObjects->TryToGetFirst();
	if (object != nullptr)
	{
		if(object->entityType == PLAYER || object->entityType == DRONE)
		{
			//object->previousPhysState = object->currentPhysState;
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
					//object->previousPhysState = object->currentPhysState;
					Integrate(object->currentPhysState, dt);
					object->currentPhysState.actingForce = Vec3(0, 0, 0);
				}
			}
		}
	}
}

//Semi-Implicit Euler Integration
void PhysicsResolver::Integrate(State &state, float dt)
{
	float maxVelocity = 0.5;
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
