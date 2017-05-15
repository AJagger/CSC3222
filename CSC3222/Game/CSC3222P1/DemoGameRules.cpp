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
	SquadAI(gameScene);
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

void DemoGameRules::SquadAI(GameScene * gameScene)
{
	DemoGameObject *jenkinsLeader = nullptr;
	DemoGameObject *tacticalLeader = nullptr;
	bool jenkinsFound = false;
	bool tacticalFound = false;

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

		//Find tactical leader
		if (object->entityType == AI && object->aiType == TACTICAL_LEADER)
		{
			tacticalLeader = object;
			tacticalFound = true;
		}

		while (gameScene->gameObjects.IsNext() && !(jenkinsFound && tacticalFound))
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

				//Find tactical leader
				if (object->entityType == AI && object->aiType == TACTICAL_LEADER)
				{
					tacticalLeader = object;
					tacticalFound = true;
				}
			}
		}
	}

	//If tatcial Leader found
	if (tacticalFound)
	{
		if (tacticalLeader->aiData.calculatedPath != nullptr && !tacticalLeader->end)
		{
			GridCoordinates targettedPosition = tacticalLeader->aiData.calculatedPath->at(tacticalLeader->aiData.currentTargetLocation);

			//Check position to see if tactical squad leader is near the center of the next tile on the path
			if ((tacticalLeader->currentPhysState.position.x - 0.5f) < ((float)targettedPosition.x + 0.1f) &&
				(tacticalLeader->currentPhysState.position.x - 0.5f) > ((float)targettedPosition.x - 0.1f) &&
				(tacticalLeader->currentPhysState.position.y - 0.5f) < ((float)targettedPosition.y + 0.1f) &&
				(tacticalLeader->currentPhysState.position.y - 0.5f) > ((float)targettedPosition.y - 0.1f))
			{
				//If tactical leader is at the end of the path, set "end" value to true so no further updates occur.
				if (tacticalLeader->aiData.currentTargetLocation == tacticalLeader->aiData.calculatedPath->size() - 1)
				{
					tacticalLeader->currentPhysState.velocity = Vec3(0, 0, 0);
					tacticalLeader->end = true;

					//If on first path, the tile beneath will be a wall. Turn wall into a gate and recalculate a new path for entering the castle.
					DemoGameObject *nextTile = gameScene->gameObjects.TryToGetFirst();
					if (nextTile != nullptr)
					{
						if (nextTile->entityType == MAP_TILE)
						{
							if (nextTile->terrainType == WALL && nextTile->currentPhysState.position.x - 0.5f == targettedPosition.x && nextTile->currentPhysState.position.y - 0.5f == targettedPosition.y - 1)
							{
								//Wall found, update the internal map representation within the pathfinder AStar object
								pathfinder.ChangeNodeProperties(GridCoordinates(nextTile->currentPhysState.position.x, nextTile->currentPhysState.position.y), GATE);
								//Also change world object properties
								nextTile->terrainType = GATE;
								nextTile->textureId = 10;

								//Set map changed flag so that paths can be recalculated
								mapChanged = true;
							}
						}

						while (gameScene->gameObjects.IsNext() && mapChanged == false)
						{
							nextTile = gameScene->gameObjects.Next();
							if (nextTile != nullptr)
							{
								if (nextTile->entityType == MAP_TILE)
								{
									if (nextTile->terrainType == WALL && nextTile->currentPhysState.position.x - 0.5f == targettedPosition.x && nextTile->currentPhysState.position.y - 0.5f == targettedPosition.y - 1)
									{
										//Wall found, update the internal map representation within the pathfinder AStar object
										pathfinder.ChangeNodeProperties(GridCoordinates(nextTile->currentPhysState.position.x, nextTile->currentPhysState.position.y), GATE);
										//Also change world object properties
										nextTile->terrainType = GATE;
										nextTile->textureId = 10;

										//Set map changed flag so that paths can be recalculated
										mapChanged = true;
									}
								}
							}
						}

					}
				}
				//else, update the aiData to target the next node in the calculated path
				else
				{
					tacticalLeader->currentPhysState.velocity = Vec3(0, 0, 0);
					tacticalLeader->aiData.currentTargetLocation = tacticalLeader->aiData.currentTargetLocation + 1;
					targettedPosition = tacticalLeader->aiData.calculatedPath->at(tacticalLeader->aiData.currentTargetLocation);
				}
			}

			//Move Tactical leader towards the center point of the next node in the path
			Vec3 force = Vec3((float)targettedPosition.x + 0.5f - tacticalLeader->currentPhysState.position.x, (float)targettedPosition.y + 0.5f - tacticalLeader->currentPhysState.position.y, 0) / 5;
			tacticalLeader->currentPhysState.actingForce.x = force.x;
			tacticalLeader->currentPhysState.actingForce.y = force.y;

			//If mapChanged flag is set, recalculate path from current location to destination
			if (mapChanged) {
				auto *newPath = pathfinder.CalculateAStarPath(tacticalLeader->aiData.calculatedPath->at(tacticalLeader->aiData.currentTargetLocation),
					GridCoordinates(tacticalLeader->aiData.destination.x, tacticalLeader->aiData.destination.y - 2), TACTICAL_PATHING);
				delete tacticalLeader->aiData.calculatedPath;
				tacticalLeader->aiData.calculatedPath = newPath;
				tacticalLeader->aiData.currentTargetLocation = 0;
				tacticalLeader->end = false;
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
				if(jenkinsLeader->aiData.currentTargetLocation == jenkinsLeader->aiData.calculatedPath->size() - 1)
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
			jenkinsLeader->currentPhysState.actingForce.x = force.x;
			jenkinsLeader->currentPhysState.actingForce.y = force.y;

			//If mapChanged flag is set, recalculate path from current location to destination
			if (mapChanged) {
				auto *newPath = pathfinder.CalculateAStarPath(jenkinsLeader->aiData.calculatedPath->at(jenkinsLeader->aiData.currentTargetLocation),
					jenkinsLeader->aiData.destination, JENKINS_PATHING);
				delete jenkinsLeader->aiData.calculatedPath;
				jenkinsLeader->aiData.calculatedPath = newPath;
				jenkinsLeader->aiData.currentTargetLocation = 0;
			}
		}
	}

	//Reset map changed flag
	mapChanged = false;
}


