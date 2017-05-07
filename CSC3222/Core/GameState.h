/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* The GameState contains information about the state of the game, including containing references to the renderer and window. This class is used to pass to
* other engine functions to give them information required to perform their tasks.
* 
* Ideally the window size information could be set from elsewhere but currently the only way to set it is through the declaration below. Default values are
* 1920x1200 with a window name of "DemoGameEngine".
*/

#pragma once
#include "../Renderer/nclgl/Window.h"
#include "../Renderer/nclglExpansion/Renderer.h"

class GameState
{
public:
	GameState();
	~GameState();

	//States
	bool paused;
	bool end;

	//Global Game Data
	Window gameWindow = Window("DemoGameEngine", 1920, 1200, false);
	Renderer renderer = Renderer(gameWindow);
};