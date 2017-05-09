/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class stores all the information about the (non-physics) game world. It has DataArrays containing all game assets (meshes, textures, collision meshes)
* and contains the physics world in which the physics simulations are performed. It contains a few functions but is mostly used to pass to other engine functions
* and provide them with the level and game data needed.
*/

#include "stdafx.h"
#include "GameScene.h"
#include "../Frameworks/DataArray.cpp" //Temp fix to Linker Error
#include "../Renderer/SOIL/SOIL.h"
#include "../Frameworks/ResouceLoader.h"

GameScene::GameScene(DataArray<Mesh*> *gameMeshes, DataArray<GLuint> *gameTextures)
{
	this->gameMeshes = gameMeshes;
	this->gameTextures = gameTextures;
}

GameScene::~GameScene()
{
	delete gameMeshes;
	delete gameTextures;
	gameMeshes = nullptr;
	gameTextures = nullptr;
}

//Use the ResourceLoader to load in the demo level.
void GameScene::LoadLevel()
{
	DemoGameObject *addObject = gameObjects.CreateNew();
	vector<char> tiles;
	ResourceLoader::LoadGridData(&tiles, "Game\\CSC3222P1\\DemoLevelData");

	//Add Background
	addObject->meshId = 2;
	addObject->textureId = 4;
	addObject->entityType = UNINITIALISED;

	//Add the tiles
	int gridNumber = 0;
	for(int y = 19; y >=0; y--)
	{
		for(int x = 0; x < 20; x++)
		{
			addObject = gameObjects.CreateNew();
			//addObject->ConfigureDefaultMapTile(0, 0);
			addObject->ConfigureDefaultMapTile(0, 2);
			addObject->currentPhysState.position = Vec3(x, y, 0.5);
			
			//Set the Tile type:
			switch(tiles.at(gridNumber))
			{
			case 'O': addObject->terrainType = OPEN_TERRAIN; addObject->textureId = 4; break;
				case 'B': addObject->terrainType = BASE_CAMP; addObject->textureId = 11; break;
				case 'C': addObject->terrainType = COVERED_TERRAIN; addObject->textureId = 5; break;
				case 'F': addObject->terrainType = FOREST; addObject->textureId = 6; break;
				case 'R': addObject->terrainType = RIVER; addObject->textureId = 7; break;
				case 'G': addObject->terrainType = GATE; addObject->textureId = 10; break;
				case 'W': addObject->terrainType = WALL; addObject->textureId = 8; break;
				case 'A': addObject->terrainType = INSIDE_CASTLE; addObject->textureId = 9; break;
			}
			gridNumber++;
		}
	}

	//Add player for testing
	addObject = gameObjects.CreateNew();
	addObject->ConfigureDefaultPlayer(1, 2);
	addObject->currentPhysState.position = Vec3(1, 1, 1);
}