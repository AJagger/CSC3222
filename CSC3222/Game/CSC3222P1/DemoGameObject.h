/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class inherits from the base GameObject class and provides a few extra functions and variables for use in the simulation
*/

#pragma once
#include <string>
#include "../../Core/GameObject.h"

enum GameEntityType
{
	MAP_TILE = 0,
	DRONE = 1,
	PLAYER = 2,
	UNINITIALISED = 3
};

enum TerrainType
{
	BASE_CAMP = 0,
	OPEN_TERRAIN = 1,
	COVERED_TERRAIN = 2,
	FOREST = 3,
	RIVER = 4,
	GATE = 5,
	WALL = 6,
	UNDEFINED_TERRAIN = 7,
	INSIDE_CASTLE = 8
};

class DemoGameObject : public GameObject
{
public:
	DemoGameObject();
	~DemoGameObject();

	GameEntityType entityType;
	TerrainType terrainType;
	bool playerControlled;

	void ConfigureDefaultMapTile(int meshId, int textureId = 0);
	void ConfigureDefaultDrone(int meshId, int textureId = 0);
	void ConfigureDefaultPlayer(int meshId, int textureId = 0);

};
