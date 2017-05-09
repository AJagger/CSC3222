/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* Basic GameObject class which contains basic information relating to game engine function. When developing a game, a class inherited from this should be
* used with more specific variables.
*/

#pragma once
#include <string>
#include "../Renderer/nclgl/Vector2.h"
#include "../Renderer/nclgl/Vector3.h"
#include "../Game/CSC3222P1/Vec3.h"

struct State
{
	//Movement data
	Vec3 position;
	Vec3 velocity;
	float mass;
	Vec3 actingForce;

	//Collision Data
	float radius;
};

class GameObject
{
public:
	GameObject();
	~GameObject();
	
	//Vector3 position;
	//Vector2 movementVector;
	//float rotation;

	State previousPhysState;
	State currentPhysState;

	int meshId;
	int textureId;
	int physMeshId;
};
