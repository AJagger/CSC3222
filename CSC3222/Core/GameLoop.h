/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* The core class of the game - the game loop
*/

/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* Minor changes to suit the purposes of this coursework
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
};
