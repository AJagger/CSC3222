/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class is used for all AI pathfinding for the Tactical and Jenkins squad leaders within coursework project 2.
*/

#pragma once
#include <queue>
#include "../../Renderer/nclgl/Vector2.h"
#include <functional>
#include "../../Core/GameObject.h"
#include "../CSC3222P1/DemoGameObject.h"


enum PathingType
{
	JENKINS_PATHING = 0,
	TACTICAL_PATHING = 1
};

struct AStarNode
{
	int nodeId;
	GridCoordinates location = GridCoordinates(0,0);
	bool passable;
	float velocityModifier;
	bool isOpenTerrain;

	float g; // cost of this node + it's predecessors
	float h; // heuristic estimate of distance to goal
	float f; // sum of cumulative cost of predecessors, self and heuristic

	int parentId;

	friend bool operator<(const AStarNode& lhs, const AStarNode& rhs)
	{
		return lhs.f < rhs.f;
	}
};

//For use with std::sort, to sort the vectors in decending order
struct CompareAStarNode {
	bool operator()(AStarNode const  *n1, AStarNode const  *n2) const
	{
		//If both f values are the same, sort by h value
		if(n1->f == n2->f)
		{
			return n1->h > n2->h;
		}

		//Else sort by f value
		return n1->f > n2->f;
	}
};

class AStar
{
public:
	AStar();
	~AStar();

	std::vector<GridCoordinates>* PerformAStar(PathingType pathingType);
	std::vector<GridCoordinates>* CalculateAStarPath(GridCoordinates start, GridCoordinates end, PathingType pathingType);
	int H(GridCoordinates start, GridCoordinates end);
	int G(int previousG, GridCoordinates directionVector, float velocityModifier);
	GridCoordinates FindNode(int id);
	void PrintSolution();
	void ChangeNodeProperties(GridCoordinates node, TerrainType terrainType);

	static const int GRID_SIZE = 20;
	AStarNode nodes[GRID_SIZE][GRID_SIZE] = {};
	std::vector<AStarNode*> openList;
	std::vector<int> openListIds;
	std::vector<AStarNode*> closedList;
	std::vector<GridCoordinates> solution;

	int startId, endId;
	GridCoordinates startLocation, endLocation;

private:
	void DeleteOpenListIds(int id);
};
