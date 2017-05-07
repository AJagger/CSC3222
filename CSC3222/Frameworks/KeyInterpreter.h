/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* Abstract class for processing captured keypresses. Should be used to create a game-specific KeyInterpreter.
*/

#pragma once
#include "../Renderer/nclgl/Keyboard.h"
#include "../Core/GameState.h"
#include "../Core/GameScene.h"
#include "../Frameworks/DataArray.h"

#include "vector"

class KeyInterpreter
{
public:
	KeyInterpreter();
	virtual ~KeyInterpreter();

	virtual void ProcessKeyPresses(std::vector<int> keys, GameState &gameState, GameScene &gameScene) = 0;
	virtual void ProcessLimitedKeys(std::vector<int> keys, GameState &gameState) = 0;
};

