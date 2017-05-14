/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class deals with removing entities (drones) from the game and terminating the game once criteria are met.
*/

#pragma once
#include "../../Core/GameScene.h"
#include "../../Core/GameState.h"

class DemoGameRules
{
public:
	DemoGameRules();
	~DemoGameRules();

	void EnactGameRules(GameScene *gameScene, GameState *gameState);

private:
	static void RemoveFinishedDrones(GameScene *gameScene);
	static bool DronesRemain(GameScene *gameScene, GameState *gameState);
	void JenkinsAI(GameScene *gameScene);

	AStar pathfinder = AStar();
};
