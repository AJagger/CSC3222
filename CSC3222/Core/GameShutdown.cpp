/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* Much of the clean-up of the game engine occurs in the destructors of objects such as GameScene and GameState. This class initiates this process by deleting the
* GameLoop and then is used for any additional shutdown (such as the audio engine).
*/

#include "stdafx.h"
#include "GameShutdown.h"

GameShutdown::GameShutdown()
{
}

GameShutdown::~GameShutdown()
{
}

void GameShutdown::TerminateGame(GameLoop* gameLoop)
{
	//Deleting the gameLoop calls the destructor 
	//Which deletes the gameState and gameScene
	//Deleting the gameScene will delete the game asset arrays: gameTextures and gameMeshes
	delete gameLoop;
}

