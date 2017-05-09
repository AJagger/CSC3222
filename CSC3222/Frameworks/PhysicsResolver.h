/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class joins the physics engine Box2D to the rest of the engine. It allows for the creation and processing of game engine objects and data into
* objects and data used by the physics engine and vica versa.
*/

#pragma once
#include "../Game/CSC3222P1/DemoGameObject.h"
#include "DataArray.h"
#include "../Game/CSC3222P1/Vec3.h"

class PhysicsResolver
{
public:
	PhysicsResolver();
	~PhysicsResolver();

	//static void SimulateActions(DataArray<DemoGameObject> *gameObjects);
	void SimulateWorld(DataArray<DemoGameObject> *gameObjects, float dt);

private:
	void Integrate(State &state, float dt);
	Vec3 Acceleration(State &state);
};