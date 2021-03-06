/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class is used for all AI pathfinding for the Tactical and Jenkins squad leaders within coursework project 2.
*/

#include "stdafx.h"
#include "AStar.h"
#include "../../Frameworks/ResouceLoader.h"

AStar::AStar()
{
	vector<char> tiles;
	ResourceLoader::LoadGridData(&tiles, "Game\\CSC3222P1\\DemoLevelData");

	//Set up the grid
	int gridNumber = 0;
	for (int y = GRID_SIZE-1; y >= 0; y--)
	{
		for (int x = 0; x < GRID_SIZE; x++)
		{
			nodes[y][x].nodeId = gridNumber;
			nodes[y][x].location = GridCoordinates(x, y);

			if(gridNumber == 1 )
			{
				startLocation = GridCoordinates(x, y);
			}
			if (gridNumber == 387)
			{
				endLocation = GridCoordinates(x, y);
			}

			//Set the Tile type:
			switch (tiles.at(gridNumber))
			{
			case 'O': nodes[y][x].passable = true; nodes[y][x].velocityModifier = 1.0f; nodes[y][x].isOpenTerrain = true; break;
			case 'B': nodes[y][x].passable = true; nodes[y][x].velocityModifier = 1.0f; nodes[y][x].isOpenTerrain = false; break;
			case 'C': nodes[y][x].passable = true; nodes[y][x].velocityModifier = 1.0f; nodes[y][x].isOpenTerrain = false; break;
			case 'F': nodes[y][x].passable = true; nodes[y][x].velocityModifier = 0.5f; nodes[y][x].isOpenTerrain = false; break;
			case 'R': nodes[y][x].passable = true; nodes[y][x].velocityModifier = 0.35f; nodes[y][x].isOpenTerrain = true; break;
			case 'G': nodes[y][x].passable = true; nodes[y][x].velocityModifier = 0.15f; nodes[y][x].isOpenTerrain = false; break;
			case 'W': nodes[y][x].passable = false; nodes[y][x].velocityModifier = 1.0f; nodes[y][x].isOpenTerrain = false; break;
			case 'A': nodes[y][x].passable = true; nodes[y][x].velocityModifier = 1.0f; nodes[y][x].isOpenTerrain = false; break;
			}
			gridNumber++;
		}
	}

	//Set default start and end ids
	startId = 1;
	endId = 387;
}

AStar::~AStar()
{
}

vector<GridCoordinates>* AStar::PerformAStar(PathingType pathingType)
{
	bool reachedEnd = false;
	bool solved = false;

	//Calculate G, H, F for start
	nodes[startLocation.y][startLocation.x].g = 0;
	nodes[startLocation.y][startLocation.x].h = H(startLocation, endLocation);
	nodes[startLocation.y][startLocation.x].f = nodes[startLocation.y][startLocation.x].g + nodes[startLocation.y][startLocation.x].h;
	openList.push_back(&nodes[startLocation.y][startLocation.x]);
	openListIds.push_back(nodes[startLocation.y][startLocation.x].nodeId);

	while(!reachedEnd)
	{
		//Check to see that the list is not empty. If it is, solution cannot be solved.
		if (openList.size() != 0)
		{
			AStarNode *p = openList.back();

			//Check to see if p is the end node
			if (p->nodeId == endId)
			{
				closedList.push_back(p);
				openList.pop_back();
				DeleteOpenListIds(p->nodeId);
				reachedEnd = true;
				solved = true;
			}
			//Add p to closed list and remove from open list
			else
			{
				closedList.push_back(p);
				openList.pop_back();
				DeleteOpenListIds(p->nodeId);
				std::sort(openList.begin(), openList.end(), CompareAStarNode());

				for (int i = 0; i < 8; i++)
				{
					GridCoordinates coodinateChangeQ = GridCoordinates(0, 0);

					switch (i)
					{
					case 0: coodinateChangeQ = GridCoordinates(0, 1); break;
					case 1: coodinateChangeQ = GridCoordinates(1, 1); break;
					case 2: coodinateChangeQ = GridCoordinates(1, 0); break;
					case 3: coodinateChangeQ = GridCoordinates(1, -1); break;
					case 4: coodinateChangeQ = GridCoordinates(0, -1); break;
					case 5: coodinateChangeQ = GridCoordinates(-1, -1); break;
					case 6: coodinateChangeQ = GridCoordinates(-1, 0); break;
					case 7: coodinateChangeQ = GridCoordinates(-1, 1); break;
					}

					//Check that the grid cell exists and can be moved through
					int updatedX = p->location.x + coodinateChangeQ.x;
					int updatedY = p->location.y + coodinateChangeQ.y;


					//Do not proceed if the grid cell is out of scope, or if the tile is not passable. If the pathing type is tactical, then proceed regardless of whether the tile is passable.
					if(updatedX >= 0 && updatedX < GRID_SIZE && updatedY >= 0 && updatedY < GRID_SIZE && (nodes[updatedY][updatedX].passable /*|| pathingType == TACTICAL_PATHING*/))
					{
						//Temp AStarNode q
						AStarNode q;
						q.nodeId = nodes[updatedY][updatedX].nodeId;
						//If tactical pathing type, check to see if the terrain is open or not and change the velocityModifier to increase the cost of moving on open terrain if needed.
						pathingType != TACTICAL_PATHING ? 
							q.g = G(p->g, coodinateChangeQ, nodes[updatedY][updatedX].velocityModifier) :
							q.g = G(p->g, coodinateChangeQ, !nodes[updatedY][updatedX].isOpenTerrain ? nodes[updatedY][updatedX].velocityModifier : nodes[updatedY][updatedX].velocityModifier * 0.2);
						q.h = H(nodes[updatedY][updatedX].location, endLocation);
						q.f = q.g + q.h;

						//Check to see if q is in the openList
						bool inOpenList = false;
						bool inClosedList = false;
						for (int ol = 0; ol < openListIds.size(); ol++)
						{
							if (openListIds.at(ol) == q.nodeId)
							{
								inOpenList = true;
								break;
							}
						}

						for (int cl = 0; cl < closedList.size(); cl++)
						{
							if (closedList.at(cl)->nodeId == q.nodeId)
							{
								inClosedList = true;
								break;
							}
						}

						if (inOpenList || inClosedList)
						{
							//If more efficient, update. Else do nothing
							if (q.g < nodes[updatedY][updatedX].g)
							{
								nodes[updatedY][updatedX].g = q.g;
								nodes[updatedY][updatedX].h = q.h;
								nodes[updatedY][updatedX].f = q.f;
								nodes[updatedY][updatedX].parentId = p->nodeId;
								std::sort(openList.begin(), openList.end(), CompareAStarNode());
							}
						}
						//If not, add to open list
						else
						{
							nodes[updatedY][updatedX].g = q.g;
							nodes[updatedY][updatedX].h = q.h;
							nodes[updatedY][updatedX].f = q.f;
							nodes[updatedY][updatedX].parentId = p->nodeId;
							openList.push_back(&nodes[updatedY][updatedX]);
							openListIds.push_back(nodes[updatedY][updatedX].nodeId);
							std::sort(openList.begin(), openList.end(), CompareAStarNode());
						}
					}
				}
			}
		}
		//If no solution, end simulation
		else
		{
			reachedEnd = true;
		}
	}

	//Solution found! (or not)
	if(solved)
	{
		vector<GridCoordinates> *returnSolution = new vector<GridCoordinates>();

		int id = endId;

		bool reachedStart = false;
		while (!reachedStart)
		{
			if(id == startId)
			{
				GridCoordinates coords = FindNode(id);
				solution.push_back(coords);
				returnSolution->push_back(coords);
				reachedStart = true;
			}
			else
			{
				GridCoordinates coords = FindNode(id);
				solution.push_back(coords);
				returnSolution->push_back(coords);
				id = nodes[coords.y][coords.x].parentId;
			}
		}

		std::reverse(solution.begin(), solution.end());
		std::reverse(returnSolution->begin(), returnSolution->end());

		//Clear lists
		openList.empty();
		closedList.empty();
		openListIds.empty();

		return returnSolution;
	}
	else
	{
		//Clear lists
		openList.empty();
		closedList.empty();
		openListIds.empty();

		return nullptr;
	}
}

vector<GridCoordinates>* AStar::CalculateAStarPath(GridCoordinates start, GridCoordinates end, PathingType pathingType)
{
	//Setup
	startLocation = start;
	endLocation = end;
	startId = nodes[start.y][start.x].nodeId;
	endId = nodes[end.y][end.x].nodeId;

	PrintSolution();

	return PerformAStar(pathingType);
}

int AStar::H(GridCoordinates start, GridCoordinates end)
{
	//Counts the squares between. Diagonal move = 14, horizontal/vertical move = 10.
	int h = 0;
	int x = end.x - start.x;
	int y = end.y - start.y;
	if (x < 0) { x = -x; }
	if (y < 0) { y = -y; }

	while(x > 0 || y > 0)
	{
		if(x > 0 && y > 0)
		{
			h += 14;
			x--;
			y--;
		}
		else if(x > 0)
		{
			h += 10;
			x--;
		}
		else if (y > 0)
		{
			h += 10;
			y--;
		}
	}

	return h;
}

int AStar::G(int previousG, GridCoordinates directionVector, float velocityModifier)
{
	//If diagonal add 14, else add 10
	if((directionVector.x == -1 || directionVector.x == 1) && (directionVector.y == -1 || directionVector.y == 1))
	{
		return previousG + (14 / velocityModifier);			//Divide by velocity modifier to increase the cost for slower tiles
	}
	else
	{
		return previousG + (10 / velocityModifier);			//Divide by velocity modifier to increase the cost for slower tiles
	}
}

GridCoordinates AStar::FindNode(int id)
{
	int x, y;
	y = (GRID_SIZE-1) - (id / GRID_SIZE);
	x = id % GRID_SIZE;
	return GridCoordinates(x,y);
}

void AStar::PrintSolution()
{
	if(solution.size() != 0)
	{
		for(int i = 0; i < solution.size(); i++)
		{
			cout << "\nStep " << i << ": (" << solution.at(i).x << "," << solution.at(i).y << ")";
		}
	}

}

void AStar::ChangeNodeProperties(GridCoordinates node, TerrainType terrainType)
{
	switch (terrainType)
	{
		case OPEN_TERRAIN: nodes[node.y][node.x].passable = true; nodes[node.y][node.x].velocityModifier = 1.0f; nodes[node.y][node.x].isOpenTerrain = true; break;
		case BASE_CAMP: nodes[node.y][node.x].passable = true; nodes[node.y][node.x].velocityModifier = 1.0f; nodes[node.y][node.x].isOpenTerrain = false; break;
		case COVERED_TERRAIN: nodes[node.y][node.x].passable = true; nodes[node.y][node.x].velocityModifier = 1.0f; nodes[node.y][node.x].isOpenTerrain = false; break;
		case FOREST: nodes[node.y][node.x].passable = true; nodes[node.y][node.x].velocityModifier = 0.5f; nodes[node.y][node.x].isOpenTerrain = false; break;
		case RIVER: nodes[node.y][node.x].passable = true; nodes[node.y][node.x].velocityModifier = 0.35f; nodes[node.y][node.x].isOpenTerrain = true; break;
		case GATE: nodes[node.y][node.x].passable = true; nodes[node.y][node.x].velocityModifier = 0.15f; nodes[node.y][node.x].isOpenTerrain = false; break;
		case WALL: nodes[node.y][node.x].passable = false; nodes[node.y][node.x].velocityModifier = 1.0f; nodes[node.y][node.x].isOpenTerrain = false; break;
		case INSIDE_CASTLE: nodes[node.y][node.x].passable = true; nodes[node.y][node.x].velocityModifier = 1.0f; nodes[node.y][node.x].isOpenTerrain = false; break;
	}
}

void AStar::DeleteOpenListIds(int id)
{
	for(int i = 0; i < openListIds.size(); i++)
	{
		if(openListIds.at(i) == id)
		{
			openListIds.erase(openListIds.begin() + i);
			break;
		}
	}
}


