#pragma once
#include "Game.h"
#include "ContentManager.h"

//Contains static constructors for preformed entities
class Constructors {
public:

	static void CreateTransform(Game * g, TransformComponent tc, PhysicsComponent pc) {
		unsigned int index = g->m_entities.add(vector<SystemBase*>{
			g->m_ts, 
			g->m_cs,
			g->m_rs
		});
		MeshStore ms = g->m_cm->GetMeshStore("cube.obj");
		g->m_ts->Create(index, tc, pc);
		g->m_cs->Create(index, cc);
		g->m_rs->Create(index, {
			g->m_cm->GetMaterial("TestMaterial"),
			ms.m_m
		});
	}
};