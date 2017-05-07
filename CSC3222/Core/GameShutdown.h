/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* Much of the clean-up of the game engine occurs in the destructors of objects such as GameScene and GameState. This class initiates this process by deleting the
* GameLoop and then is used for any additional shutdown (such as the audio engine).
*/

#pragma once
#include "GameLoop.h"

static class GameShutdown
{
public:
	GameShutdown();
	~GameShutdown();

	static void TerminateGame(GameLoop *gameLoop);
};