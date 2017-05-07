/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class inherits from the base GameObject class and provides a few extra functions and variables for use in the game demo.
* GameEntityType in particular is very useful for determining exactly what type of object it is.
*/

#pragma once
#include <string>
#include "../../Core/GameObject.h"

enum GameEntityType
{
	STATIC_OBJECT = 0,
	NPC = 1,
	PLAYER = 2,
	PROJECTILE = 3,
	EFFECT = 4,
	UNINITIALISED = 5
};

class DemoGameObject : public GameObject
{
public:
	DemoGameObject();
	DemoGameObject(GameEntityType type, int meshId, int textureId = 0);
	~DemoGameObject();

	GameEntityType entityType;
	bool hasTarget;					// hasTarget &
	int targetObjectId;				// targetObjectId used for NPCs & Projectiles
	int lifeTime;					// lifeTime used for Effects

	void ConfigureDefaultStatic(int meshId, int textureId = 0);
	void ConfigureDefaultNPC(int meshId, int textureId = 0);
	void ConfigureDefaultPlayer(int meshId, int textureId = 0);
	void ConfigureDefaultProjectile(int meshId, int textureId = 0);
	void ConfigureDefaultEffect(int meshId, int textureId = 0);

};
