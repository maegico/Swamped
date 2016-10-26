#include "Game.h"

Game::Game() {
	//initialize entity list and systems
	m_entities = FreeVector<vector<ISystem*>>();
	m_ts = new CollisionSystem();
	m_ts2 = new TestSystem2();
}

Game::~Game() {
	//delete system objects
	delete m_ts;
	delete m_ts2;
}

void Game::Update() {
	m_ts->Update(this);
	m_ts2->Update(this);
}

void Game::RemoveEntity(unsigned int entityId) {
	//bounds check
	if (m_entities.size() > entityId)
	{
		//call remove on the entity ID for each system associated with the entity
		vector<ISystem*> systems = m_entities[entityId];
		for (ISystem* s : systems) {
			s->Remove(entityId);
		}
		//free the ID in entity list
		m_entities.free(entityId);
	}
}