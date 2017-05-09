/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class joins the physics engine Box2D to the rest of the engine. It allows for the creation and processing of game engine objects and data into
* objects and data used by the physics engine and vica versa.
*/

#pragma once
#include "../Game/CSC3222P1/DemoGameObject.h"
/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class acts as the world - updating positions/velocities, storing spring information and calling collision detection systems
*/

#include "DataArray.h"
#include "../Game/CSC3222P1/Vec3.h"
#include "Collision.h"

struct Spring
{
	int parentId, childId;
	DemoGameObject *parentObject;
	DemoGameObject *childObject;
};

class PhysicsResolver
{
public:
	PhysicsResolver();
	~PhysicsResolver();

	void SimulateWorld(DataArray<DemoGameObject> *gameObjects, float dt);
	void AddSpring(DataArray<DemoGameObject> *gameObjects, int parentId, int childId);
	void RemoveSpring(int id);

private:
	DataArray<Spring> springs = DataArray<Spring>();
	DataArray<DetectedCollision> collisions = DataArray<DetectedCollision>();

	static void Integrate(State &state, float dt);
	static Vec3 Acceleration(State &state);
	static Vec3 DetermineSpringForce(DemoGameObject *parentObject, DemoGameObject *childObject);
};