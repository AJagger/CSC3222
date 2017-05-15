/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class implements the abstract KeyInterpreter class. It is responsible for processing collected user keypresses.
* It supports dealing with multiple keypresses and multiple player controlled objects.
*
* Currently the key configuration is stored in an enum in the header file. This is not ideal but it is functional.
*
* The key presses are collected using the Keyboard class included in the ncgl library and they are referred to by values declared in
* an enum in the Keyboard class.
*/

/* CSC3222 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class deals with removing entities (drones) from the game and terminating the game once criteria are met.
*/

#pragma once
#include "../../Frameworks/KeyInterpreter.h"
#include "../../Frameworks/DataArray.h"
#include "../../Core/GameScene.h"
#include "../../Core/GameState.h"
#include "DemoGameObject.h"

class DemoKeyInterpreter : public KeyInterpreter
{
public:
	DemoKeyInterpreter();
	~DemoKeyInterpreter();

	void ProcessKeyPresses(std::vector<int> keys, GameState &gameState, GameScene &gameScene) override;
	void ProcessLimitedKeys(std::vector<int> keys, GameState &gameState) override;
	
private:
	enum KeyConfiguration
	{
		CONFIG_TOGGLE_PAUSE = KEYBOARD_ESCAPE,
		CONFIG_PLAYER_FORWARD = KEYBOARD_W,
		CONFIG_PLAYER_BACKWARD = KEYBOARD_S,
		CONFIG_PLAYER_LEFT = KEYBOARD_D,
		CONFIG_PLAYER_RIGHT = KEYBOARD_A,
		CONFIG_FORCE_END = KEYBOARD_C,
		CONFIG_DEBUG_TOGGLE = KEYBOARD_B,
		CONFIG_START_SIMULATION = KEYBOARD_K,
		CONFIG_START_JENKINS = KEYBOARD_J,
		CONFIG_START_TACTICAL = KEYBOARD_L
	};

	void Force_End(GameState &gamestate);
	void Toggle_Pause(GameState &gamestate);
	void Player_Forward(GameScene &gamescene);
	void Player_Backward(GameScene &gamescene);
	void Player_Left(GameScene &gamescene);
	void Player_Right(GameScene &gamescene);
	void Debug_Toggle(GameState &gamestate);
	void StartSim(GameState &gamestate, GameScene &gamescene);
	void StartJenkins(GameState &gamestate, GameScene &gamescene);
	void StartTactical(GameState & gamestate, GameScene & gamescene);
	int DetermineOrientation(std::vector<int> keys);

	vector<DemoGameObject*> controlledEntities;
};