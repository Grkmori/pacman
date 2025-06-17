
#include <iostream>

#include "game/Game.hpp"
#include "model/Network.hpp"


int main()
{
	std::cout << "+++ Starting the game." << std::endl;
	Game game;
	Network nn;

	game.Initialize(nn);

	game.Run(nn);

	return 0;
}
