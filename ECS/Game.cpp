#include "Game.h"

//initialize entity list and systems
Game::Game() {
	m_entities = FreeVector<vector<SystemBase*>>();
	m_cs = new CollisionSystem();
	m_ts = new TransformSystem();
}

//delete system objects
Game::~Game() {
	delete m_cs;
	delete m_ts;
}

//Advances the game in time
void Game::Update(float dt) {
	m_ts->Update(this, dt);
	m_cs->Update(this, dt);
}

//Removes an entity from all its systems
void Game::RemoveEntity(unsigned int entityId) {
	//bounds check
	if (m_entities.size() > entityId)
	{
		//call remove on the entity ID for each system associated with the entity
		vector<SystemBase*> systems = m_entities[entityId];
		for (SystemBase* s : systems) {
			s->Remove(entityId);
		}
		//free the ID in entity list
		m_entities.free(entityId);
	}
}