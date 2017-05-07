/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class implements the abstract KeyInterpreter class. It is responsible for processing collected user keypresses.
* It supports dealing with multiple keypresses and multiple player controlled objects.
* 
* Currently the key configuration is stored in an enum in the header file. This is not ideal but it is functional.
* 
* The key presses are collected using the Keyboard class included in the ncgl library and they are referred to by values declared in
* an enum in the Keyboard class.
*/

#include "stdafx.h"
#include "DemoKeyInterpreter.h"
#include "../../Frameworks/DataArray.cpp" //Temp fix to Linker Errors

DemoKeyInterpreter::DemoKeyInterpreter()
{
}

DemoKeyInterpreter::~DemoKeyInterpreter()
{
}

void DemoKeyInterpreter::ProcessKeyPresses(std::vector<int> keys, GameState &gameState, GameScene &gameScene)
{
	//If no key presses then do nothing
	if (!keys.empty())
	{
		//Look for controlled entities and store Ids in vector. Saves lookups when processing multiple keypresses.
		DemoGameObject *returnedEntity = gameScene.gameObjects.TryToGetFirst();
		if (returnedEntity != nullptr)	//Continue only if there is a returned item (i.e. don't try to do anything if there are no objects in the data structure)
		{
			if (returnedEntity->playerControlled)
			{
				controlledEntities.push_back(returnedEntity);
			}

			while (gameScene.gameObjects.IsNext())
			{
				returnedEntity = gameScene.gameObjects.Next();
				if (returnedEntity->playerControlled)
				{
					controlledEntities.push_back(returnedEntity);
				}
			}

			//If player controlled entities have been found, proceed to processing the keypresses
			if (controlledEntities.size() > 0)
			{
				//Loop through keypresses and determine what function to perform
				for (int i = 0; i < keys.size(); i++)
				{
					switch (keys[i])
					{
					case CONFIG_FORCE_END: Force_End(gameState); break;
					case CONFIG_TOGGLE_PAUSE: Toggle_Pause(gameState); break;
					case CONFIG_PLAYER_FORWARD:	Player_Forward(gameScene); break;
					case CONFIG_PLAYER_BACKWARD: Player_Backward(gameScene); break;
					case CONFIG_PLAYER_LEFT: Player_Left(gameScene); break;
					case CONFIG_PLAYER_RIGHT: Player_Right(gameScene); break;
					case CONFIG_DEBUG_TOGGLE: Debug_Toggle(gameState); break;
					}
				}

				//Deal with setting orientation separately since it can be influenced by multiple keys being pressed at once
				int orientation = DetermineOrientation(keys);
				for (int i = 0; i < controlledEntities.size(); i++)
				{
					if (controlledEntities[i]->entityType == PLAYER)
					{
						controlledEntities[i]->rotation = orientation;
					}
				}
			}

			//Clear the controlled entities vector
			controlledEntities.clear();
		}
	}

}

//For use when the logic of the game loop is unaccessable - e.g. when the game is paused so that keys can be pressed to unpause.
void DemoKeyInterpreter::ProcessLimitedKeys(std::vector<int> keys, GameState & gameState)
{
	//Loop through keypresses and determine what function to perform
	for (int i = 0; i < keys.size(); i++)
	{
		switch (keys[i])
		{
			case CONFIG_FORCE_END: Force_End(gameState); break;
			case CONFIG_TOGGLE_PAUSE: Toggle_Pause(gameState); break;
			case CONFIG_DEBUG_TOGGLE: Debug_Toggle(gameState); break;
		}
	}
}

void DemoKeyInterpreter::Force_End(GameState & gamestate)
{
	gamestate.end = true;
}

void DemoKeyInterpreter::Toggle_Pause(GameState & gamestate)
{
	gamestate.paused = !gamestate.paused;
}

void DemoKeyInterpreter::Player_Forward(GameScene & gamescene)
{
	for (int i = 0; i < controlledEntities.size(); i++)
	{
		if (controlledEntities[i]->entityType == PLAYER)
		{
			controlledEntities[i]->movementVector.y = controlledEntities[i]->movementVector.y + 0.005f;
		}
	}
}

void DemoKeyInterpreter::Player_Backward(GameScene & gamescene)
{
	for (int i = 0; i < controlledEntities.size(); i++)
	{
		if (controlledEntities[i]->entityType == PLAYER)
		{
			controlledEntities[i]->movementVector.y = controlledEntities[i]->movementVector.y - 0.005f;
		}
	}
}

void DemoKeyInterpreter::Player_Left(GameScene & gamescene)
{
	for (int i = 0; i < controlledEntities.size(); i++)
	{
		if (controlledEntities[i]->entityType == PLAYER)
		{
			controlledEntities[i]->movementVector.x = controlledEntities[i]->movementVector.x + 0.005f;
		}
	}
}

void DemoKeyInterpreter::Player_Right(GameScene & gamescene)
{
	for (int i = 0; i < controlledEntities.size(); i++)
	{
		if (controlledEntities[i]->entityType == PLAYER)
		{
			controlledEntities[i]->movementVector.x = controlledEntities[i]->movementVector.x - 0.005f;
		}
	}
}

void DemoKeyInterpreter::Debug_Toggle(GameState & gamestate)
{
	gamestate.renderer.ToggleBlendMode();
}

int DemoKeyInterpreter::DetermineOrientation(std::vector<int> keys)
{
	int rotation = 0;
	bool forward = false, left = false, right = false , backward = false;

	for(int i = 0; i < keys.size(); i++)
	{
		if (keys[i] == CONFIG_PLAYER_FORWARD) { forward = true; }
		if (keys[i] == CONFIG_PLAYER_LEFT) { left = true; }
		if (keys[i] == CONFIG_PLAYER_RIGHT) { right = true; }
		if (keys[i] == CONFIG_PLAYER_BACKWARD) { backward = true; }
	}

	//Basic rotations for just one key being pressed
	if (left) { rotation = 270; }
	if (right) { rotation = 90; }
	if (forward) { rotation = 0; }
	if (backward) { rotation = 180; }

	//Check for combinations and override if found
	if (forward && left) { rotation = 315; }
	if (forward && right) { rotation = 45; }
	if (forward && backward) { rotation = 0; }
	if (backward && left) { rotation = 225; }
	if (backward && right) { rotation = 135; }
	if (left && right && forward) { rotation = 0; }
	if (left && right && backward) { rotation = 180; }

	return rotation;
}

