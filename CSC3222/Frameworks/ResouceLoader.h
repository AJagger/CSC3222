/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class deals with loading data (textures/meshes/levels (scenes)/collision meshes) into the game engine from files stored in the game directories.
*/

#pragma once
#include "DataArray.h"
//#include "gl/glew.h"
#include "../Renderer/GLEW/include/GL/glew.h"
#include <vector>
#include "../Renderer/nclgl/Mesh.h"
#include "../Game/CSC3222P1/DemoGameObject.h"
#include "PhysicsResolver.h"

class ResourceLoader
{
public:
	ResourceLoader();
	~ResourceLoader();
	
	static bool LoadTextures(DataArray<GLuint> *textures, const string & texDirPath);
	static bool LoadMeshes(DataArray<Mesh*> *meshes, const string & meshDirPath);
	static bool LoadObjectList(DataArray<DemoGameObject> *gameObjects, const string &sceneFile);
	//static bool LoadCollisionMeshes(DataArray<CollisionMesh> *gameCollisionMeshes, const string &meshDirPath);
	//static CollisionMesh ImportCollisionMesh(const string &collisionMeshPath);

private:
	static const int BUFFER_SIZE = 200;
};

