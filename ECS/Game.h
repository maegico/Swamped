#pragma once
#include "CollisionSystem.h"
#include "TransformSystem.h"
#include "RenderingSystem.h"
#include "ContentManager.h"
#include "DXCore.h"
#include <vector>

using namespace std;

//The game instance. Not a singleton
class Game : public DXCore {
public:
	//Systems
	CollisionSystem m_cs;
	TransformSystem m_ts;
	RenderingSystem m_rs;

	ContentManager m_cm;

	Game(HINSTANCE hInstance);
	~Game();

	void Init();

	//Update for the game. The program's main loop will call this
	void Update(float dT, float totalTime);

	//Frees an entity's components, and frees its entry in m_entities
	void QueueRemoveEntity(unsigned int entityId);

	friend class Constructors;
private:
	//Associates systems with entity IDs for deletion
	FreeVector<vector<SystemBase*>> m_entities;
	vector<unsigned int> m_removeQueue;
	void UpdateTitleBarForGame(std::string in);
};
