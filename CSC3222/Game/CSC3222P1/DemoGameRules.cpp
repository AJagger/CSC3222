/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class deals with removing entities (drones) from the game and terminating the game once criteria are met.
*/

#include "stdafx.h"
#include "DemoGameRules.h"
#include "../CSC3222/Frameworks/DataArray.cpp"

DemoGameRules::DemoGameRules()
{
}

DemoGameRules::~DemoGameRules()
{
}

void DemoGameRules::EnactGameRules(GameScene * gameScene, GameState * gameState)
{
	RemoveFinishedDrones(gameScene);
	if(gameState->simulationStarted)
	{
		gameState->end = DronesRemain(gameScene, gameState);
	}
}

void DemoGameRules::RemoveFinishedDrones(GameScene * gameScene)
{
	//Cylce through objects
	DemoGameObject *object = gameScene->gameObjects.TryToGetFirst();
	if (object != nullptr)
	{
		//Check drones and remove if they have reached the end
		if (object->entityType == DRONE && object->end)
		{
			int id = gameScene->gameObjects.GetId(*object);
			gameScene->gameObjects.Free(id);
			gameScene->physicsWorld.RemoveSpring(id);
		}

		while (gameScene->gameObjects.IsNext())
		{
			object = gameScene->gameObjects.Next();
			if (object != nullptr)
			{
				//Check drones and remove if they have reached the end
				if (object->entityType == DRONE && object->end)
				{
					int id = gameScene->gameObjects.GetId(*object);
					gameScene->gameObjects.Free(id);
					gameScene->physicsWorld.RemoveSpring(id);
				}
			}
		}
	}
}

bool DemoGameRules::DronesRemain(GameScene * gameScene, GameState * gameState)
{
	int droneCount = 0;

	//Cylce through objects
	DemoGameObject *object = gameScene->gameObjects.TryToGetFirst();
	if (object != nullptr)
	{
		//Count drones
		if (object->entityType == DRONE)
		{
			droneCount++;
		}

		while (gameScene->gameObjects.IsNext())
		{
			object = gameScene->gameObjects.Next();
			if (object != nullptr)
			{
				//Count drones
				if (object->entityType)
				{
					droneCount++;
				}
			}
		}
	}

	if(droneCount == 2)
	{
		return true;
	}

	return false;
}


