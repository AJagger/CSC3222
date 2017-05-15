This second submission includes the code for projects 1 and 2.

Since the framework I am using is based very heavily on the game engine written for CSC3224, the files that have been created or received major changes for coursework 1 & 2 of CSC3222
have comments at the top identifying them. Files created/Modified for project 2:

New:
AStar class (Game/CSC3222P2)

Significant changes:
GameScene class (Core folder)
ResourceLoader class (Framework folder)
DemoKeyInterpreter class (Game/CSC3222P1)
GameObject class (Core folder)
DemoGameObject class (Game/CSC3222P1)
DemoGameRules class (Game/CSC3222P1)

+ Resouce files for the coursework 2 added to the DemoMeshes/DemoTextures/DemoLevelData folders within Game/CSC3222P1


OVERVIEW:

The creation of the squads is handled within the GameScene class. The AStar path is calculated using the AStar class. The movement control associated with the squads
and the breeching logic used by the tactical squad is handled through the DemoGameRules class (movement still uses the numerical integration from project 1 to determine
velocities etc.).

Jenkins squad:
Spawns on a random base tile, paths into the square behind the castle gates using the fastest route.

Tactical squad:
Spawns on a random base tile, paths to a randomly selected tile infront of a wall tile. Once this tile is reached, the wall infront is turned into a gate and a new
path is calculated which brings the tactical squad through the hole in the wall and into the castle.
When generating a path for a tactical squad, the AStar algorithm significantly increases the "G" cost (distance from start) for tiles which are in the open.

Both squads:
Squads are held together using springs connecting the squad leaders to the soldiers (referred to as "drones" in the code) as was done for project 1.
Tile movement speeds are handled by changing the "G" cost (distance from start) within the AStar algorithm depending on the velocity multiplier for the tile. Velocity modifiers
which slow the squad more have a higher "G" cost.

As with Project 1:
The solution will attempt to exit cleanly but breaks during shutdown.