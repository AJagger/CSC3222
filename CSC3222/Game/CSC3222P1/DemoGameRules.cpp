/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* A very basic implementation of a class dealing with game-specific logic. For an actual game, the victory conditions would be a lot more complex but for the demo
* the simple victory condition implemented is enough.
* This class is called in the game loop and is intended to contain game logic in addition to the victory conditions in the game for coursework 2.
*/

#include "stdafx.h"
#include "DemoGameRules.h"

DemoGameRules::DemoGameRules()
{
}

DemoGameRules::~DemoGameRules()
{
}

void DemoGameRules::EnactGameRules(GameScene * gameScene, GameState * gameState)
{
	//For the demo project the only game specific rule to be checked and acted upon is whether the victory conditions have been met
	if (CheckVictoryConditions(gameScene, gameState))
	{
		gameState->end = true;
	}
}

bool DemoGameRules::CheckVictoryConditions(GameScene * gameScene, GameState * gameState)
{
	//Victory Conditions listed below:
	Vector3 victoryPosition = Vector3(24, 12, 0);


	bool victory = false;

	//Search through scene entities looking for player controlled entities. Once found, check if their position to see if they have completed the level
	DemoGameObject *returnedEntity = gameScene->gameObjects.TryToGetFirst();
	if (returnedEntity != nullptr)	//Continue only if there is a returned item (i.e. don't try to do anything if there are no objects in the data structure)
	{
		if (returnedEntity->playerControlled)
		{
			if (returnedEntity->position.x >= victoryPosition.x && returnedEntity->position.y >= victoryPosition.y)
			{
				victory = true; //Player craft has reached the end of the demo maze!
			}
		}

		while (gameScene->gameObjects.IsNext())
		{
			returnedEntity = gameScene->gameObjects.Next();
			if (returnedEntity->playerControlled)
			{
				if (returnedEntity->position.x >= victoryPosition.x && returnedEntity->position.y >= victoryPosition.y)
				{
					victory = true; //Player craft has reached the end of the demo maze!
				}
			}
		}
	}

	return victory;
}
