#include "Game.h"
#include "Constructors.h"
#include <time.h>
#include <iostream>
#include <string>

int main() {
	srand(time(nullptr));
	//create game
	Game g = Game();

	//create some objects with static constructors
	//Constructors::CreateTestObject(&g, { 1 }, { 'c' });

	//example update and removal
	for (unsigned int c = 0; c < 2000; c++)
	{
		g.Update();
		if(c%10==0)
			std::cout <<to_string(c) +" - "+to_string(g.m_ts->GetCount())+"/"+to_string(g.m_ts->GetSize())<< std::endl;
	}
	//g.RemoveEntity(0);

	return 0;
}