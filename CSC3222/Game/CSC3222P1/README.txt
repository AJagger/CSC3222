This folder contains all the c++ files being used specifically by the game rather than the game engine - in this case the DemoCode game.

Also in this folder are the game assets:

	-DemoCollisionMeshes stores all CollisionMeshes to be imported and provided to Box2D for physics simulations
	-DemoMeshes stores all mesh data used to render objects through the ncgl library
	-DemoTextures stores all textures applied to rendered objects
	-DemoLevelData stores representations of game levels which can be loaded and played.

Each of these asset folders has a file indicating which files to include when the engine tries to load the assets in with the ResourceLoader class.

All of the items in this DemoCode directory have been created solely by myself, Aidan Jagger 130281034

...with the exception of some of the textures :)