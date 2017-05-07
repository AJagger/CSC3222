/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* The core class of the game - the game loop
*/

#pragma once
#include "GameScene.h"
#include "GameState.h"
#include "../Frameworks/PhysicsResolver.h"

class GameLoop
{
public:
	GameLoop(GameState *gameState, GameScene *gameScene);
	~GameLoop();

	void RunLoop();
	void SetTickLength(int tickLength);

private:
	GameState *state;
	GameScene *scene;
	vector<int> input;

	unsigned int lastTick;
	int tickLength;

	void TempPositionUpdater(); //For testing purposes to allow the game loop to run without a physics engine


};
