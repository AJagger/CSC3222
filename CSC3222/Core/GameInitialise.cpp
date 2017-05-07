/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* Basic static class to initialise the various Game objects and systems.
*/

#include "stdafx.h"
#include "GameInitialise.h"
#include "../Frameworks/DataArray.h"
#include "../Frameworks/DataArray.cpp" //Temp fix to Visual Studio linker errors
#include "GameObject.h"
#include "GameState.h"
#include "GameScene.h"
#include "../Frameworks/ResouceLoader.h"
#include "../Frameworks/PhysicsResolver.h"

GameInitialise::GameInitialise()
{
}

GameInitialise::~GameInitialise()
{
}

//Create the DataArrays for storing game assets and assemble the GameLoop
GameLoop* GameInitialise::InitialiseGame()
{
	//Initialise WWise Audio
	if (InitSoundEngine()) {

		//Create the Game State. This initialises the renderer and GLEW
		GameState* gameState = new GameState();

		//Initialise DataArrays of textures & Meshes contained within the game
		DataArray<Mesh*> *gameMeshes = new DataArray<Mesh*>();
		DataArray<GLuint> *gameTextures = new DataArray<GLuint>();

		if(ResourceLoader::LoadMeshes(gameMeshes, "Game\\DemoCode\\DemoMeshes") &&
			ResourceLoader::LoadTextures(gameTextures, "Game\\DemoCode\\DemoTextures"))
		{
			//Initialise Game Scene & load test level
			GameScene* gameScene = new GameScene(gameMeshes, gameTextures);
			gameScene->LoadLevel("Game\\DemoCode\\DemoLevelData\\DemoScene.csv");

			GameLoop* gameLoop = new GameLoop(gameState, gameScene);
			return gameLoop;
		}

		//If initialiser has reached this point then an error has occured. Clean up created objects and then return null
		delete gameMeshes;
		delete gameTextures;
		delete gameState;
		gameMeshes = nullptr;
		gameTextures = nullptr;
		gameState = nullptr;
	}
}

bool GameInitialise::InitSoundEngine()
{
	return false;
}


