/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* The Draw class joins the game logic and objects used in the rest of the engine with the logic and objects used in the renderer. This class is responsible
* for converting and selecting the specific data needed to render the gameObjects and providing it to the Renderer.
*/

#pragma once
#include "DataArray.h"
#include "../Game/CSC3222P1/DemoGameObject.h"
#include "../Core/GameState.h"
#include "../nclglExpansion/Renderer.h"
#include <gl/glew.h>
#include "../Core/GameScene.h"

class Draw
{
public:
	Draw();
	~Draw();

	static void RenderObjects(GameScene *scene, GameState *state);
};

