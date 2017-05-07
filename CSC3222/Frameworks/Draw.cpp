/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* The Draw class joins the game logic and objects used in the rest of the engine with the logic and objects used in the renderer. This class is responsible 
* for converting and selecting the specific data needed to render the gameObjects and providing it to the Renderer.
*/

#include "stdafx.h"
#include "Draw.h"
#include "../Frameworks/DataArray.cpp" //Temp fix to Linker Errors

Draw::Draw()
{
}


Draw::~Draw()
{
}

//Loop through the GameObjects, extract the render data and add it to the Renderer pipeline.
void Draw::RenderObjects(GameScene *scene, GameState *state)
{
	vector<DrawData> renderData = vector<DrawData>();
	DataArray<DemoGameObject> *objects = &scene->gameObjects;

	DemoGameObject *object = objects->TryToGetFirst();
	if(object != nullptr)
	{
		state->renderer.AddToPipeline(*scene->gameMeshes->TryToGet(object->meshId), *scene->gameTextures->TryToGet(object->textureId), object->position, object->rotation);

		while(objects->IsNext())
		{
			object = objects->Next();
			if (object != nullptr)
			{
				state->renderer.AddToPipeline(*scene->gameMeshes->TryToGet(object->meshId), *scene->gameTextures->TryToGet(object->textureId), object->position, object->rotation);
			}
		}
	}

	state->renderer.RenderScene();

}
