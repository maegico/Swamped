#pragma once
#include "Game.h"
#include "ContentManager.h"

//Contains static constructors for preformed entities
class Constructors {
public:

	static void CreateTransform(Game * g, TransformComponent tc, PhysicsComponent pc, CollisionMask cm) {
		//get entityID and add system list to game
		unsigned int index = g->m_entities.add(vector<SystemBase*>{
			g->m_ts, 
			g->m_cs,
			g->m_rs
		});

		//get mesh and bounding box
		MeshStore ms = g->m_cm->GetMeshStore("cube.obj");
		//copy collision mask into bounding box
		ms.m_bb.m_cm = cm;

		//create components
		g->m_ts->Create(index, tc, pc);
		g->m_cs->Create(index, ms.m_bb);
		g->m_rs->Create(index, {
			g->m_cm->GetMaterial("TestMaterial"),
			ms.m_m
		});
	}
};