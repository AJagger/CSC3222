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
	JenkinsAI(gameScene);
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

void DemoGameRules::JenkinsAI(GameScene * gameScene)
{
	DemoGameObject *jenkinsLeader = nullptr;
	bool jenkinsFound = false;

	//Cylce through objects
	DemoGameObject *object = gameScene->gameObjects.TryToGetFirst();
	if (object != nullptr)
	{
		//Find Jenkins leader
		if (object->entityType == AI && object->aiType == JENKINS_LEADER)
		{
			jenkinsLeader = object;
			jenkinsFound = true;
		}

		while (gameScene->gameObjects.IsNext() && !jenkinsFound)
		{
			object = gameScene->gameObjects.Next();
			if (object != nullptr)
			{
				//Find Jenkins leader
				if (object->entityType == AI && object->aiType == JENKINS_LEADER)
				{
					jenkinsLeader = object;
					jenkinsFound = true;
				}
			}
		}
	}

	//If Jenkins Leader found
	if(jenkinsFound)
	{
		if (jenkinsLeader->aiData.calculatedPath != nullptr && !jenkinsLeader->end)
		{
			GridCoordinates targettedPosition = jenkinsLeader->aiData.calculatedPath->at(jenkinsLeader->aiData.currentTargetLocation);
			
			//Check position to see if Jenkin's squad leader is near the center of the next tile on the path
			if((jenkinsLeader->currentPhysState.position.x - 0.5f) < ((float)targettedPosition.x + 0.1f) &&
				(jenkinsLeader->currentPhysState.position.x - 0.5f) > ((float)targettedPosition.x - 0.1f) &&
				(jenkinsLeader->currentPhysState.position.y - 0.5f) < ((float)targettedPosition.y + 0.1f) &&
				(jenkinsLeader->currentPhysState.position.y - 0.5f) > ((float)targettedPosition.y - 0.1f))
			{
				//If Jenkin's leader is at the end of the path, set "end" value to true so no further updates occur.
				if(jenkinsLeader->aiData.currentTargetLocation == jenkinsLeader->aiData.calculatedPath->size())
				{
					jenkinsLeader->currentPhysState.velocity = Vec3(0, 0, 0);
					jenkinsLeader->end = true;
				}
				//else, update the aiData to target the next node in the calculated path
				else
				{
					jenkinsLeader->currentPhysState.velocity = Vec3(0, 0, 0);
					jenkinsLeader->aiData.currentTargetLocation = jenkinsLeader->aiData.currentTargetLocation + 1;
					targettedPosition = jenkinsLeader->aiData.calculatedPath->at(jenkinsLeader->aiData.currentTargetLocation);
				}
			}

			//Move Jenkin's leader towards the center point of the next node in the path
			Vec3 force = Vec3((float)targettedPosition.x + 0.5f - jenkinsLeader->currentPhysState.position.x, (float)targettedPosition.y + 0.5f - jenkinsLeader->currentPhysState.position.y, 0) / 5;
			//jenkinsLeader->currentPhysState.velocity.x = force.x * 5;
			//jenkinsLeader->currentPhysState.velocity.y = force.y * 5;
			jenkinsLeader->currentPhysState.actingForce.x = force.x;
			jenkinsLeader->currentPhysState.actingForce.y = force.y;
			//jenkinsLeader->currentPhysState.position.x = targettedPosition.x + 0.5f;
			//jenkinsLeader->currentPhysState.position.y = targettedPosition.y + 0.5f;

			//Else, if leader has reached that tile, move to next location.

			//jenkinsLeader->aiData.calculatedPath = pathfinder.CalculateAStarPath(jenkinsLeader->aiData.)
		}
	}


}


