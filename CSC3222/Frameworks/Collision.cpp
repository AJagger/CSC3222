/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* Detects and resolved Collisions. Also has methods for checking the terrain that an object is on.
*/

#include "stdafx.h"
#include "Collision.h"
#include <cmath>

Collision::Collision()
{
}

Collision::~Collision()
{
}

void Collision::IndividualCheck(DataArray<DemoGameObject>* gameObjects, DataArray<DetectedCollision>* collisions, DemoGameObject * initialObject)
{
	//Cylce through objects
	DemoGameObject *object = gameObjects->TryToGetFirst();
	Vec3 overlapDistance = Vec3(0, 0, 0);
	int wallcollisions = 0;

	if (object != nullptr)
	{
		//Only perform checks on objects that can collide. In all of this, the initialObject is assumed to be a circle
		//since in this implementation, the only objects which can move are circles.
		if (object != initialObject) {

			//Drone vs Player
			if (object->entityType == PLAYER)
			{
				overlapDistance = CircleCircleCheck(initialObject, object);
				if (overlapDistance != Vec3(0, 0, 0))
				{
					ResolveCollisionDronePlayer(initialObject, object);
				}
			}

			//Drone vs Drone
			else if (object->entityType == DRONE && initialObject->entityType != PLAYER)
			{
				overlapDistance = CircleCircleCheck(initialObject, object);
				if (overlapDistance != Vec3(0, 0, 0))
				{
					ResolveCollisionDroneDrone(initialObject, object);
				}
			}

			//Player/drone vs wall
			else if (object->entityType == MAP_TILE && object->terrainType == WALL/* && initialObject->entityType == PLAYER*/)
			{
				overlapDistance = CircleSquareCheck(initialObject, object);
				if (overlapDistance != Vec3(0, 0, 0))
				{
					//If dealing with multiple wall collisions, begin to add forces
					//This prevents objects from tunneling between wall blocks
					ResolveCollisionWall(object, initialObject, overlapDistance, wallcollisions != 0);
					wallcollisions++;
				}
			}
		}

		while (gameObjects->IsNext())
		{
			object = gameObjects->Next();
			if (object != nullptr)
			{
				//Only perform checks on objects that can collide. In all of this, the initialObject is assumed to be a circle
				//since in this implementation, the only objects which can move are circles.
				if (object != initialObject) {

					//Drone vs Player
					if (object->entityType == PLAYER)
					{
						overlapDistance = CircleCircleCheck(initialObject, object);
						if (overlapDistance != Vec3(0, 0, 0))
						{
							ResolveCollisionDronePlayer(initialObject, object);
						}
					}

					//Drone vs Drone
					else if (object->entityType == DRONE && initialObject->entityType != PLAYER)
					{
						overlapDistance = CircleCircleCheck(initialObject, object);
						if (overlapDistance != Vec3(0, 0, 0))
						{
							ResolveCollisionDroneDrone(initialObject, object);
						}
					}

					//Player/drone vs wall
					else if (object->entityType == MAP_TILE && object->terrainType == WALL/* && initialObject->entityType == PLAYER*/)
					{
						overlapDistance = CircleSquareCheck(initialObject, object);
						if (overlapDistance != Vec3(0, 0, 0))
						{
							//If dealing with multiple wall collisions, begin to add forces
							//This prevents objects from tunneling between wall blocks
							ResolveCollisionWall(object, initialObject, overlapDistance, wallcollisions != 0);
							wallcollisions++;
						}
					}
				}
			}
		}
	}
}

void Collision::DetectTerrain(DataArray<DemoGameObject>* gameObjects)
{
	//Cylce through objects
	DemoGameObject *object = gameObjects->TryToGetFirst();
	if (object != nullptr)
	{
		//Only perform checks on objects that can move
		if (object->entityType == PLAYER || object->entityType == DRONE)
		{
			CheckTerrain(gameObjects, object);

			//Due to the way DataArray works, to get the next element after iterating through from the start in IndividualCheck()
			//we need to request the gameObject from the dataArray so that the internal counter is reset to this position.
			int checkedId = gameObjects->GetId(*object);
			object = gameObjects->TryToGet(checkedId);
		}

		while (gameObjects->IsNext())
		{
			object = gameObjects->Next();
			if (object != nullptr)
			{
				//Only perform checks on objects that can move
				if (object->entityType == PLAYER || object->entityType == DRONE)
				{
					CheckTerrain(gameObjects, object);

					//Due to the way DataArray works, to get the next element after iterating through from the start in IndividualCheck()
					//we need to request the gameObject from the dataArray so that the internal counter is reset to this position.
					int checkedId = gameObjects->GetId(*object);
					object = gameObjects->TryToGet(checkedId);
				}
			}
		}
	}
}

void Collision::CheckTerrain(DataArray<DemoGameObject>* gameObjects, DemoGameObject * initialObject)
{
	//Cylce through objects
	DemoGameObject *object = gameObjects->TryToGetFirst();
	bool found = false;

	if (object != nullptr)
	{
		//Only perform checks on Tiles.
		if (object != initialObject) {
			if(object->entityType == MAP_TILE)
			{
				//if object is within a certain boundary
				if(-3 >(initialObject->currentPhysState.position - object->currentPhysState.position).MagnitudeXY() < 3)
				{
					//Determing if object is within the square
					float squareFarLeft = object->currentPhysState.position.x - (object->currentPhysState.radius);
					float squareFarRight = object->currentPhysState.position.x + (object->currentPhysState.radius);
					float squareTop = object->currentPhysState.position.y + (object->currentPhysState.radius);
					float squareBottom = object->currentPhysState.position.y - (object->currentPhysState.radius);


					if (initialObject->currentPhysState.position.x >= squareFarLeft && initialObject->currentPhysState.position.x <= squareFarRight 
						&& initialObject->currentPhysState.position.y >= squareBottom && initialObject->currentPhysState.position.y <= squareTop)
					{
						//initial object is within this object
						initialObject->currentPhysState.velocityModifier = object->currentPhysState.velocityModifier;
						if(object->terrainType == INSIDE_CASTLE)
						{
							initialObject->end = true;
						}
						found = true;
					}					
				}
			}
		}

		while (gameObjects->IsNext() && !found)
		{
			object = gameObjects->Next();
			if (object != nullptr)
			{
				//Only perform checks on Tiles.
				if (object != initialObject) {
					if (object->entityType == MAP_TILE)
					{
						//if object is within a certain boundary
						if (-3 >(initialObject->currentPhysState.position - object->currentPhysState.position).MagnitudeXY() < 3)
						{
							//Determing if object is within the square
							float squareFarLeft = object->currentPhysState.position.x - (object->currentPhysState.radius);
							float squareFarRight = object->currentPhysState.position.x + (object->currentPhysState.radius);
							float squareTop = object->currentPhysState.position.y + (object->currentPhysState.radius);
							float squareBottom = object->currentPhysState.position.y - (object->currentPhysState.radius);


							if (initialObject->currentPhysState.position.x >= squareFarLeft && initialObject->currentPhysState.position.x <= squareFarRight
								&& initialObject->currentPhysState.position.y >= squareBottom && initialObject->currentPhysState.position.y <= squareTop)
							{
								//initial object is within this object
								initialObject->currentPhysState.velocityModifier = object->currentPhysState.velocityModifier;
								if (object->terrainType == INSIDE_CASTLE)
								{
									initialObject->end = true;
								}
								found = true;
							}
						}
					}
				}
			}
		}
	}
}

void Collision::CheckCollisions(DataArray<DemoGameObject>* gameObjects, DataArray<DetectedCollision>* collisions)
{
		//Cylce through objects
		DemoGameObject *object = gameObjects->TryToGetFirst();
		if (object != nullptr)
		{
			//Only perform checks on objects that can move
			if (object->entityType == PLAYER || object->entityType == DRONE)
			{
				IndividualCheck(gameObjects, collisions, object);

				//Due to the way DataArray works, to get the next element after iterating through from the start in IndividualCheck()
				//we need to request the gameObject from the dataArray so that the internal counter is reset to this position.
				int checkedId = gameObjects->GetId(*object);
				object = gameObjects->TryToGet(checkedId);
			}

			while (gameObjects->IsNext())
			{
				object = gameObjects->Next();
				if (object != nullptr)
				{
					//Only perform checks on objects that can move
					if (object->entityType == PLAYER || object->entityType == DRONE)
					{
						IndividualCheck(gameObjects, collisions, object);

						//Due to the way DataArray works, to get the next element after iterating through from the start in IndividualCheck()
						//we need to request the gameObject from the dataArray so that the internal counter is reset to this position.
						int checkedId = gameObjects->GetId(*object);
						object = gameObjects->TryToGet(checkedId);
					}
				}
			}
		}
}

Vec3 Collision::CircleCircleCheck(DemoGameObject * object1, DemoGameObject * object2)
{
	//AABB check - If AABB collision, do more detailed look
	if (object1->currentPhysState.position.x + object1->currentPhysState.radius + object2->currentPhysState.radius > object2->currentPhysState.position.x
		&& object1->currentPhysState.position.x < object2->currentPhysState.position.x + object1->currentPhysState.radius + object2->currentPhysState.radius
		&& object1->currentPhysState.position.y + object1->currentPhysState.radius + object2->currentPhysState.radius > object2->currentPhysState.position.y
		&& object1->currentPhysState.position.y < object2->currentPhysState.position.y + object1->currentPhysState.radius + object2->currentPhysState.radius) 
	{
		float distance = (object2->currentPhysState.position - object1->currentPhysState.position).MagnitudeXY();
		if(distance < (object1->currentPhysState.radius + object2->currentPhysState.radius))
		{
			//Collision has taken place, return distance of overlap
			Vec3 distanceVector = object2->currentPhysState.position - object1->currentPhysState.position;
			distanceVector.x = 0;
			Vec3 direction = distanceVector.normalise();

			return distanceVector - (direction*distance);
		}
	}

	//If no collision, return 0
	return Vec3(0, 0, 0);
}

//Code used is partially taken from my CSC3221 coursework
Vec3 Collision::CircleSquareCheck(DemoGameObject * circle, DemoGameObject * square)
{
	//First check to see if the square and circle are near each other
	float squareOuterBoundingCircleRadius = sqrt(((square->currentPhysState.radius)*(square->currentPhysState.radius)) + ((square->currentPhysState.radius)*(square->currentPhysState.radius)));
	float distanceBetweenOrigins = (float)sqrt(((circle->currentPhysState.position.x - square->currentPhysState.position.x)*(circle->currentPhysState.position.x - square->currentPhysState.position.x))
												+ ((circle->currentPhysState.position.y - square->currentPhysState.position.y)*(circle->currentPhysState.position.y - square->currentPhysState.position.y)));
	float combinedOuterRadii = circle->currentPhysState.radius + squareOuterBoundingCircleRadius;

	//If outer bounding circle of the square does not collide with the circle then they are too far appart to collide
	if (distanceBetweenOrigins > combinedOuterRadii)
	{
		return Vec3(0,0,0);
	}

	//If objects could be colliding, find the outer point of the circle on the line between the centre of the circle and centre of the square
	Vec3 magnitudeVector = (circle->currentPhysState.position - square->currentPhysState.position);
	magnitudeVector.z = 0;
	magnitudeVector = -magnitudeVector.normalise();
	Vec3 circlePoint = (circle->currentPhysState.position + 0.5*(magnitudeVector*circle->currentPhysState.radius));

	//And determine if this point is within the square
	float squareFarLeft = square->currentPhysState.position.x - (square->currentPhysState.radius);
	float squareFarRight = square->currentPhysState.position.x + (square->currentPhysState.radius);
	float squareTop = square->currentPhysState.position.y + (square->currentPhysState.radius);
	float squareBottom = square->currentPhysState.position.y - (square->currentPhysState.radius);


	if (circlePoint.x >= squareFarLeft && circlePoint.x <= squareFarRight && circlePoint.y >= squareBottom && circlePoint.y <= squareTop)
	{
		//Circle and square are overlapping. Return resolution vector:
		return magnitudeVector;
	}
	else
	{
		return Vec3(0,0,0);
	}
}

//Resolves collisions between the drone and player by creating a spring between the two which only acts on the drone
void Collision::ResolveCollisionDronePlayer(DemoGameObject *drone, DemoGameObject *player)
{
	//Creates a spring between the two objects to pull them apart
	float springRestingLength = drone->currentPhysState.radius + player->currentPhysState.radius;
	float k = 100;
	float b = 0.1;

	//Vector displacement
	Vec3 x = drone->currentPhysState.position - player->currentPhysState.position;
	x.z = 0;	//Set the z value to 0 since this is a 2D usage of a 3D vector
	float distance = x.MagnitudeXY();

	//Relative velocity
	Vec3 v = drone->currentPhysState.velocity - player->currentPhysState.velocity;

	//F = -k(|x|-d)(x/|x|) - bv
	drone->currentPhysState.actingForce =  -k*(distance - springRestingLength)*(x.normalise()) - b*v;
}

//Resolves collisions between two drones by creating a spring between the two which only acts on both drones
void Collision::ResolveCollisionDroneDrone(DemoGameObject * object1, DemoGameObject * object2)
{
	//Creates a spring between the two objects to pull them apart
	float springRestingLength = object1->currentPhysState.radius + object2->currentPhysState.radius;
	float k = 100;
	float b = 0.1;

	//Vector displacement
	Vec3 x = object1->currentPhysState.position - object2->currentPhysState.position;
	x.z = 0;	//Set the z value to 0 since this is a 2D usage of a 3D vector
	float distance = x.MagnitudeXY();

	//Relative velocity
	Vec3 v = object1->currentPhysState.velocity - object2->currentPhysState.velocity;

	//F = -k(|x|-d)(x/|x|) - bv
	object1->currentPhysState.actingForce = -k*(distance - springRestingLength)*(x.normalise()) - b*v;
	object2->currentPhysState.actingForce = -k*-(distance - springRestingLength)*(x.normalise()) - b*v;
}

void Collision::ResolveCollisionWall(DemoGameObject * wall, DemoGameObject * object, Vec3 overlapVector, bool addForces)
{
	//Creates a spring between the two objects to push them apart
	float k = 0.1;
	float b = 5; //5

	//Vector displacement
	Vec3 x = object->currentPhysState.position - wall->currentPhysState.position;
	x.z = 0;	//Set the z value to 0 since this is a 2D usage of a 3D vector
	float distance = x.MagnitudeXY();

	//Relative velocity
	Vec3 v = object->currentPhysState.velocity - wall->currentPhysState.velocity;

	//F = -k(|x|-d)(x/|x|) - bv
	if(addForces)
	{
		object->currentPhysState.actingForce += -k*(overlapVector)-b*v;
	}
	else
	{
		object->currentPhysState.actingForce = -k*(overlapVector)-b*v;
	}

}
