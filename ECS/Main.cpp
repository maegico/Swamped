#include "Game.h"
#include "Constructors.h"

int main() {
	//create game
	Game g = Game();

	//create some objects with static constructors
	Constructors::CreateTestObject(&g, { 1 }, { 'c' });

	//example update and removal
	g.Update();
	g.RemoveEntity(0);

	return 0;
}