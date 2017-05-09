/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class inherits from the base GameObject class and provides a few extra functions and variables for use in the simulation
*/

#include "stdafx.h"
#include "DemoGameObject.h"

DemoGameObject::DemoGameObject()
{
	entityType = UNINITIALISED;
	terrainType = UNDEFINED_TERRAIN;
	currentPhysState.position = Vec3(0, 0, 0);
	currentPhysState.velocity = Vec3(0, 0, 0);
	currentPhysState.actingForce = Vec3(0, 0, 0);
	currentPhysState.mass = 1;
	currentPhysState.velocityModifier = 1.0;
	playerControlled = false;

	meshId = 0;
	textureId = 0;
	physMeshId = 0;
}

DemoGameObject::~DemoGameObject()
{
}

void DemoGameObject::ConfigureDefaultMapTile(int meshId, int textureId)
{
	entityType = MAP_TILE;
	terrainType = UNDEFINED_TERRAIN;

	this->meshId = meshId;
	this->textureId = textureId;
	physMeshId = 0;
}

void DemoGameObject::ConfigureDefaultDrone(int meshId, int textureId)
{
	entityType = DRONE;
	terrainType = UNDEFINED_TERRAIN;

	this->meshId = meshId;
	this->textureId = textureId;
	physMeshId = 0;
}

void DemoGameObject::ConfigureDefaultPlayer(int meshId, int textureId)
{
	entityType = PLAYER;
	terrainType = UNDEFINED_TERRAIN;

	playerControlled = true;

	this->meshId = meshId;
	this->textureId = textureId;
	physMeshId = 0;
}
