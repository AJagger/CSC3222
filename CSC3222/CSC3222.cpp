// CSC3222.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Core/GameShutdown.h"
#include "Core/GameInitialise.h"


int main()
{
	GameLoop *game = GameInitialise::InitialiseGame();
	if (game)
	{
		game->RunLoop();
		GameShutdown::TerminateGame(game);
	}
	else
	{
		//Something ****ed up
	}
}

