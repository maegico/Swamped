#include "Game.h"
#include "Constructors.h"
#include <time.h>
#include <iostream>
#include <string>

int main() {
	time_t now = time(nullptr);
	srand(now);
	//create game
	Game g = Game();

	//create some objects with static constructors
	//Constructors::CreateTestObject(&g, { 1 }, { 'c' });

	//example update and removal
	for (unsigned int c = 0; c < 2000; c++)
	{
		g.Update(1.0f);
		if(c%10==0)
			std::cout <<to_string(c) +" - "+to_string(g.m_ts->GetCount())+"/"+to_string(g.m_ts->GetSize()) +", "+ to_string(time(nullptr) - now) << std::endl;
	}
	//g.RemoveEntity(0);

	return 0;
}