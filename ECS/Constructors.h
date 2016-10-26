#pragma once
#include "Game.h"

//Contains static constructors for preformed entities
class Constructors {
public:
	//Example constructor
	static void CreateTestObject(Game * g, TestComponent tc, TestComponent2 tc2) {
		//add a list of systems this entity has components from to Game::m_entities
		//m_entities is a FreeVector, so this will give us the ID of the new entity
		unsigned int index = g->m_entities.add(vector<ISystem*>{g->m_ts, g->m_ts2});

		//create components, one for each system
		g->m_ts->Create(index, tc);
		g->m_ts2->Create(index, tc2);
	}
};