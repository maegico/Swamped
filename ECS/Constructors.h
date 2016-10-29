#pragma once
#include "Game.h"

//Contains static constructors for preformed entities
class Constructors {
public:

	static void CreateTransform(Game * g, TransformComponent tc, PhysicsComponent pc, BoundingBox cc) {
		unsigned int index = g->m_entities.add(vector<ISystem*>{
			g->m_ts, 
			g->m_cs
		});

		g->m_ts->Create(index, tc, pc);
		g->m_cs->Create(index, cc);
	}
};