#include "Game.h"

Game::Game() {
	//initialize entity list and systems
	m_entities = FreeVector<vector<ISystem*>>();
	m_cs = new CollisionSystem();
	m_ts = new TransformSystem();
}

Game::~Game() {
	//delete system objects
	delete m_cs;
	delete m_ts;
}

void Game::Update() {
	m_cs->Update(this);
	m_ts->Update(this);
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