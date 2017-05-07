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

class GameObject
{
public:
	GameObject();
	GameObject(bool AIEnabled, bool physicsEnabled, std::string entityName);
	~GameObject();

	bool playerControlled;
	bool hostile;
	bool AIEnabled;
	bool physicsEnabled;
	bool collisionsEnabled;
	bool staticObject;
	std::string entityName;
	
	Vector3 position;
	Vector2 movementVector;
	float rotation;

	int meshId;
	int textureId;
	int physMeshId;
};
