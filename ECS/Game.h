#pragma once
#include "TestSystem.h"
#include <vector>

using namespace std;

//The game instance. Not a singleton
class Game {
public:
	//Example system
	CollisionSystem * m_ts;
	//Example system
	TestSystem2 * m_ts2;

	Game();
	~Game();

	//Update for the game. The program's main loop will call this
	void Update();

	//Frees an entity's components, and frees its entry in m_entities
	void RemoveEntity(unsigned int entityId);

	friend class Constructors;
private:
	//Associates systems with entity IDs for deletion
	FreeVector<vector<ISystem*>> m_entities;
};
