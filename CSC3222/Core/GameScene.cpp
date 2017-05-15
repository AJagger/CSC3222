/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class stores all the information about the (non-physics) game world. It has DataArrays containing all game assets (meshes, textures, collision meshes)
* and contains the physics world in which the physics simulations are performed. It contains a few functions but is mostly used to pass to other engine functions
* and provide them with the level and game data needed.
*/

/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class has been pretty much entirely rewritten to add functions related to add functionality required for CSC3222 coursework P1.
*/

#include "stdafx.h"
#include "GameScene.h"
#include "../Frameworks/DataArray.cpp" //Temp fix to Linker Error
#include "../Renderer/SOIL/SOIL.h"
#include "../Frameworks/ResouceLoader.h"
#include "../Game/CSC3222P2/AStar.h"

GameScene::GameScene(DataArray<Mesh*> *gameMeshes, DataArray<GLuint> *gameTextures)
{
	this->gameMeshes = gameMeshes;
	this->gameTextures = gameTextures;
	physicsWorld = PhysicsResolver();
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
	addObject->currentPhysState.position.z = -1;

	//Add the tiles
	int gridNumber = 0;
	for(int y = 19; y >=0; y--)
	{
		for(int x = 0; x < 20; x++)
		{
			addObject = gameObjects.CreateNew();
			//addObject->ConfigureDefaultMapTile(0, 0);
			addObject->ConfigureDefaultMapTile(0, 2);
			addObject->currentPhysState.position = Vec3(x+0.5, y+0.5, 0);
			addObject->currentPhysState.radius = 0.55;
			
			//Set the Tile type:
			switch(tiles.at(gridNumber))
			{
			case 'O': addObject->terrainType = OPEN_TERRAIN; addObject->textureId = 4; addObject->currentPhysState.velocityModifier = 1.0f; break;
				case 'B': addObject->terrainType = BASE_CAMP; addObject->textureId = 11; addObject->currentPhysState.velocityModifier = 1.0f; break;
				case 'C': addObject->terrainType = COVERED_TERRAIN; addObject->textureId = 5; addObject->currentPhysState.velocityModifier = 1.0f; break;
				case 'F': addObject->terrainType = FOREST; addObject->textureId = 6; addObject->currentPhysState.velocityModifier = 0.5f; break;
				case 'R': addObject->terrainType = RIVER; addObject->textureId = 7; addObject->currentPhysState.velocityModifier = 0.35f; break;
				case 'G': addObject->terrainType = GATE; addObject->textureId = 10; addObject->currentPhysState.velocityModifier = 0.15f; break;
				case 'W': addObject->terrainType = WALL; addObject->textureId = 8; addObject->currentPhysState.velocityModifier = 1.0f; break;
				case 'A': addObject->terrainType = INSIDE_CASTLE; addObject->textureId = 9; addObject->currentPhysState.velocityModifier = 1.0f; break;
			}
			gridNumber++;
		}
	}
}

void GameScene::AddEntities()
{
	//Select random base square
	srand(time(NULL));
	int chosenBase = rand() % 18;
	int currentBase = 0;
	Vec3 basePosition = Vec3(0, 0, 0);
	bool found = false;

	//Cylce through objects
	DemoGameObject *object = gameObjects.TryToGetFirst();
	if (object != nullptr)
	{
		//If object is a base tile
		if (object->terrainType == BASE_CAMP)
		{
			if(currentBase == chosenBase)
			{
				basePosition = object->currentPhysState.position;
				found = true;
			}
			currentBase++;
		}

		while (gameObjects.IsNext() && !found)
		{
			object = gameObjects.Next();
			if (object != nullptr)
			{
				//If object is a base tile
				if (object->terrainType == BASE_CAMP)
				{
					if (currentBase == chosenBase)
					{
						basePosition = object->currentPhysState.position;
						found = true;
					}
					currentBase++;
				}
			}
		}
	}


	DemoGameObject *addObject = gameObjects.CreateNew();
	vector<DemoGameObject*> createdDrones = vector<DemoGameObject*>();
	for (int i = 0; i < 9; i++)
	{
		//Add drone
		addObject = gameObjects.CreateNew();
		addObject->ConfigureDefaultDrone(1, 1);
		//addObject->currentPhysState.position = Vec3(0.5*(i + 1), 0.5*(i + 1), 0.05*(i + 1));
		addObject->currentPhysState.position = Vec3(basePosition.x, basePosition.y, 0.05*(i + 1));
		addObject->currentPhysState.radius = 0.2;

		createdDrones.push_back(addObject);
	}

	//Add player
	DemoGameObject *playerObject = gameObjects.CreateNew();
	playerObject->ConfigureDefaultPlayer(1, 2);
	playerObject->currentPhysState.position = Vec3(basePosition.x, basePosition.y, 1);
	playerObject->currentPhysState.radius = 0.2;

	//Add springs
	for (int i = 0; i < 9; i++)
	{
		physicsWorld.AddSpring(&gameObjects, gameObjects.GetId(*playerObject), gameObjects.GetId(*createdDrones.at(i)));
	}
}

void GameScene::AddJenkinsSquad()
{
	//Select random base square
	srand(time(NULL));
	int chosenBase = rand() % 18;
	int currentBase = 0;
	Vec3 basePosition = Vec3(0, 0, 0);
	bool found = false;

	//Cylce through objects
	DemoGameObject *object = gameObjects.TryToGetFirst();
	if (object != nullptr)
	{
		//If object is a base tile
		if (object->terrainType == BASE_CAMP)
		{
			if (currentBase == chosenBase)
			{
				basePosition = object->currentPhysState.position;
				found = true;
			}
			currentBase++;
		}

		while (gameObjects.IsNext() && !found)
		{
			object = gameObjects.Next();
			if (object != nullptr)
			{
				//If object is a base tile
				if (object->terrainType == BASE_CAMP)
				{
					if (currentBase == chosenBase)
					{
						basePosition = object->currentPhysState.position;
						found = true;
					}
					currentBase++;
				}
			}
		}
	}


	DemoGameObject *addObject = gameObjects.CreateNew();
	vector<DemoGameObject*> createdDrones = vector<DemoGameObject*>();
	for (int i = 0; i < 9; i++)
	{
		//Add drone
		addObject = gameObjects.CreateNew();
		addObject->ConfigureDefaultDrone(1, 1);
		addObject->currentPhysState.position = Vec3(basePosition.x + ((float)i/10 - 0.4f), basePosition.y + ((float)i / 10 - 0.4f), 0.05*(i + 1));
		addObject->currentPhysState.radius = 0.2;

		createdDrones.push_back(addObject);
	}

	//Add Jenkins Leader
	DemoGameObject *playerObject = gameObjects.CreateNew();
	playerObject->ConfigureDefaultPlayer(1, 12);
	playerObject->entityType = AI;
	playerObject->aiType = JENKINS_LEADER;
	playerObject->currentPhysState.position = Vec3(basePosition.x, basePosition.y, 1);
	playerObject->currentPhysState.radius = 0.2;
	
	AStar pathfinder = AStar();
	playerObject->aiData.origin = GridCoordinates(basePosition.x, basePosition.y);
	playerObject->aiData.destination = GridCoordinates(7, 0);
	playerObject->aiData.calculatedPath = pathfinder.CalculateAStarPath(playerObject->aiData.origin, playerObject->aiData.destination, JENKINS_PATHING);

	//Add springs
	for (int i = 0; i < 9; i++)
	{
		physicsWorld.AddSpring(&gameObjects, gameObjects.GetId(*playerObject), gameObjects.GetId(*createdDrones.at(i)));
	}
}

void GameScene::AddTacticalSquad()
{
	//Select random base square
	srand(time(NULL));
	int chosenBase = rand() % 18;
	int currentBase = 0;
	Vec3 basePosition = Vec3(0, 0, 0);
	bool found = false;

	//Cylce through objects
	DemoGameObject *object = gameObjects.TryToGetFirst();
	if (object != nullptr)
	{
		//If object is a base tile
		if (object->terrainType == BASE_CAMP)
		{
			if (currentBase == chosenBase)
			{
				basePosition = object->currentPhysState.position;
				found = true;
			}
			currentBase++;
		}

		while (gameObjects.IsNext() && !found)
		{
			object = gameObjects.Next();
			if (object != nullptr)
			{
				//If object is a base tile
				if (object->terrainType == BASE_CAMP)
				{
					if (currentBase == chosenBase)
					{
						basePosition = object->currentPhysState.position;
						found = true;
					}
					currentBase++;
				}
			}
		}
	}

	//Select random wall to break through
	int chosenWall = rand() % 10;
	int currentWall = 0;
	Vec3 wallPosition = Vec3(0, 0, 0);
	bool wallFound = false;

	//Cylce through objects
	DemoGameObject *wall = gameObjects.TryToGetFirst();
	if (wall != nullptr)
	{
		//If object is a wall
		if (wall->terrainType == WALL)
		{
			if (currentWall == chosenWall)
			{
				wallPosition = wall->currentPhysState.position;
				wallFound = true;
			}
			currentWall++;
		}

		while (gameObjects.IsNext() && !wallFound)
		{
			wall = gameObjects.Next();
			if (wall != nullptr)
			{
				//If object is a wall
				if (wall->terrainType == WALL)
				{
					if (currentWall == chosenWall)
					{
						wallPosition = wall->currentPhysState.position;
						wallFound = true;
					}
					currentWall++;
				}
			}
		}
	}


	DemoGameObject *addObject = gameObjects.CreateNew();
	vector<DemoGameObject*> createdDrones = vector<DemoGameObject*>();
	for (int i = 0; i < 9; i++)
	{
		//Add drone
		addObject = gameObjects.CreateNew();
		addObject->ConfigureDefaultDrone(1, 1);
		addObject->currentPhysState.position = Vec3(basePosition.x + ((float)i / 10 - 0.4f), basePosition.y + ((float)i / 10 - 0.4f), 0.05*(i + 1));
		addObject->currentPhysState.radius = 0.2;

		createdDrones.push_back(addObject);
	}

	//Add Tactical Leader
	DemoGameObject *playerObject = gameObjects.CreateNew();
	playerObject->ConfigureDefaultPlayer(1, 13);
	playerObject->entityType = AI;
	playerObject->aiType = TACTICAL_LEADER;
	playerObject->currentPhysState.position = Vec3(basePosition.x, basePosition.y, 1);
	playerObject->currentPhysState.radius = 0.2;

	AStar pathfinder = AStar();
	playerObject->aiData.origin = GridCoordinates(basePosition.x, basePosition.y);
	//playerObject->aiData.destination = GridCoordinates(12, 1);
	playerObject->aiData.destination = GridCoordinates(wallPosition.x, wallPosition.y + 1); //+1 to Y so that path is found to tile above.
	playerObject->aiData.calculatedPath = pathfinder.CalculateAStarPath(playerObject->aiData.origin, playerObject->aiData.destination, TACTICAL_PATHING);

	//Add springs
	for (int i = 0; i < 9; i++)
	{
		physicsWorld.AddSpring(&gameObjects, gameObjects.GetId(*playerObject), gameObjects.GetId(*createdDrones.at(i)));
	}
}