/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* Basic GameObject class which contains basic information relating to game engine function. When developing a game, a class inherited from this should be
* used with more specific variables.
*/

/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* Additions of a lot of physics-related data for use in the PhysicsResolver class.
*/

#pragma once
#include <string>
#include "../Renderer/nclgl/Vector2.h"
#include "../Renderer/nclgl/Vector3.h"
#include "../Game/CSC3222P1/Vec3.h"
#include "../Game/CSC3222P2/AStar.h"

struct State
{
	enum CollisionObject
	{
		COLLISION_CIRCLE = 0,
		COLLISION_SQUARE = 1
	};

	//Movement data
	Vec3 position;
	Vec3 velocity;
	float mass;
	Vec3 actingForce;
	float velocityModifier;

	//Collision Data
	CollisionObject collisionObject;
	float radius;
};

struct AIData
{
	GridCoordinates origin;
	GridCoordinates destination;
	std::vector<GridCoordinates> *calculatedPath;
	int currentTargetLocation = 0;
};

class GameObject
{
public:
	GameObject();
	~GameObject();

	State currentPhysState;
	AIData aiData;
	bool end = false;

	int meshId;
	int textureId;
	int physMeshId;
};
