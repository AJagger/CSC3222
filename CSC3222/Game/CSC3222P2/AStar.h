#pragma once
#include <queue>
#include "../../Renderer/nclgl/Vector2.h"
#include <functional>


struct GridCoordinates
{
	int x, y;

	GridCoordinates() { x=0; y=0; }
	GridCoordinates(int x, int y) { this->x = x; this->y = y; }
};

struct AStarNode
{
	int nodeId;
	GridCoordinates location = GridCoordinates(0,0);
	bool passable;
	float velocityModifier;

	float g; // cost of this node + it's predecessors
	float h; // heuristic estimate of distance to goal
	float f; // sum of cumulative cost of predecessors and self and heuristic

	int parentId;

	friend bool operator<(const AStarNode& lhs, const AStarNode& rhs)
	{
		return lhs.f < rhs.f;
	}
};

struct CompareAStarNode {
	bool operator()(AStarNode const  &n1, AStarNode const  &n2) {
		// return "true" if "p1" is ordered before "p2", for example:
		return n1.f < n2.f;
	}
};

class AStar
{
public:
	AStar();
	~AStar();

	std::vector<GridCoordinates>* PerformAStar();
	std::vector<GridCoordinates>* CalculateAStarPath(GridCoordinates start, GridCoordinates end);
	int H(GridCoordinates start, GridCoordinates end);
	int G(int previousG, GridCoordinates directionVector);
	GridCoordinates FindNode(int id);
	void PrintSolution();

	static const int GRID_SIZE = 20;
	AStarNode nodes[GRID_SIZE][GRID_SIZE] = {};
	std::priority_queue<AStarNode, std::vector<AStarNode>, CompareAStarNode> openList;
	std::vector<int> openListIds;
	std::vector<AStarNode> closedList;
	std::vector<GridCoordinates> solution;

	int startId, endId;
	GridCoordinates startLocation, endLocation;

private:
	void DeleteOpenListIds(int id);
};
