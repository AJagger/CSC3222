/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* Detects and resolved Collisions. Also has methods for checking the terrain that an object is on.
*/

#pragma once
#include "../Game/CSC3222P1/DemoGameObject.h"
#include "DataArray.h"
#include "../Game/CSC3222P1/Vec3.h"

struct DetectedCollision
{
	DemoGameObject *object1;
	DemoGameObject *object2;
	float lengthOfOverlap;
	Vec3 resolutionForce1;
	Vec3 resolutionForce2;
};

class Collision
{
public:
	Collision();
	~Collision();

	static void CheckCollisions(DataArray<DemoGameObject> *gameObjects, DataArray<DetectedCollision> *collisions);
	static void IndividualCheck(DataArray<DemoGameObject> *gameObjects, DataArray<DetectedCollision> *collisions, DemoGameObject *initialObject);
	static void DetectTerrain(DataArray<DemoGameObject> *gameObjects);
	static void CheckTerrain(DataArray<DemoGameObject> *gameObjects, DemoGameObject *initialObject);

	static Vec3 CircleCircleCheck(DemoGameObject *object1, DemoGameObject *object2);
	static Vec3 CircleSquareCheck(DemoGameObject *circle, DemoGameObject *square);
	static void ResolveCollisionDronePlayer(DemoGameObject *drone, DemoGameObject *player);
	static void ResolveCollisionDroneDrone(DemoGameObject *object1, DemoGameObject *object2);
	static void ResolveCollisionWall(DemoGameObject *wall, DemoGameObject *object, Vec3 overlapVector, bool addForces);
};