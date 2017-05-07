/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* The GameState contains information about the state of the game, including containing references to the renderer and window. This class is used to pass to
* other engine functions to give them information required to perform their tasks.
*/

#include "stdafx.h"
#include "GameState.h"

GameState::GameState()
{
	paused = false;
	end = false;
}

GameState::~GameState()
{
}
