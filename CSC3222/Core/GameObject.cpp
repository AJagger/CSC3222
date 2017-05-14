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

#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
	if(aiData.calculatedPath != nullptr)
	{
		delete aiData.calculatedPath;
	}
}
