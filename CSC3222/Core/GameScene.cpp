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

//Use the ResourceLoader to load in a level from a file.
void GameScene::LoadLevel(const string &sceneFile)
{
	ResourceLoader::LoadObjectList(&gameObjects, sceneFile);

	//Iterate through the game objects that have been added and add them into the physics simulation if required.
	DemoGameObject *returnedEntity = gameObjects.TryToGetFirst();
	if (returnedEntity != nullptr)	//Continue only if there is a returned item (i.e. don't try to do anything if there are no objects in the data structure)
	{
		if (returnedEntity->collisionsEnabled)
		{
			//CreateRelatedPhysicsObject(gameObjects.GetId(*returnedEntity));
		}

		while (gameObjects.IsNext())
		{
			returnedEntity = gameObjects.Next();
			if (returnedEntity->collisionsEnabled)
			{
				//CreateRelatedPhysicsObject(gameObjects.GetId(*returnedEntity));
			}
		}
	}
}


//Very basic test scene used in early development
void GameScene::LoadTestLevel()
{
	//DemoGameObject *obj = gameObjects.CreateNew();
	////Mesh *objMesh = Mesh::GenerateTriangle();
	//Mesh *objMesh = *gameMeshes->TryToGet(1);
	////MAKE THIS RELATIVE
	//objMesh->SetTexture(SOIL_load_OGL_texture("C:\\Users\\Aidan\\Documents\\Visual Studio 2015\\Projects\\3224Engine\\3224Engine\\Game\\DemoCode\\DemoTextures\\uc.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
	//obj->ConfigureDefaultPlayer(objMesh);

	//obj = gameObjects.CreateNew();
	//objMesh = Mesh::GenerateTriangle();
	////MAKE THIS RELATIVE
	//objMesh->SetTexture(SOIL_load_OGL_texture("C:\\Users\\Aidan\\Documents\\Visual Studio 2015\\Projects\\3224Engine\\3224Engine\\Game\\DemoCode\\DemoTextures\\brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
	//obj->ConfigureDefaultStatic(objMesh);
	//obj->position = Vector2(1,1);

	//obj = gameObjects.CreateNew();
	//objMesh = Mesh::LoadMeshFile("C:\\Users\\Aidan\\Documents\\Visual Studio 2015\\Projects\\3224Engine\\3224Engine\\Game\\DemoCode\\DemoMeshes\\square.asciimesh");
	//objMesh->SetTexture(SOIL_load_OGL_texture("C:\\Users\\Aidan\\Documents\\Visual Studio 2015\\Projects\\3224Engine\\3224Engine\\Game\\DemoCode\\DemoTextures\\uc.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
	//obj->ConfigureDefaultStatic(objMesh);
	//obj->position = Vector2(3, 3);

	DemoGameObject *obj = gameObjects.CreateNew();
	int meshId = 0;
	int textureId = 1;
	obj->ConfigureDefaultPlayer(meshId, textureId);

	obj = gameObjects.CreateNew();
	meshId = 1;
	textureId = 2;
	obj->ConfigureDefaultStatic(meshId, textureId);
	obj->position = Vector3(1, 1, 0);

	obj = gameObjects.CreateNew();
	meshId = 1;
	obj->ConfigureDefaultStatic(meshId);
	obj->position = Vector3(3, 3, 0);
}
