#include "Game.h"

int main()
{
	srand(time(NULL));

	Game game("Configs/config.ini");

	game.run();
	return 0;
}
//skin change