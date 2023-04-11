#include "Game.h"

// TODO:  Separate engine code from game code.

int main(int argc, char* argv[])
{
	Game game;
	bool success = game.Init();
	if (success)
	{
		game.Run();
	}
	game.Quit();
	return 0;
}
