#pragma once
#include "Game.h"
#include "ContentManager.h"

using namespace DirectX;
//Contains static constructors for preformed entities
class Constructors {
public:
	static double fRand(double min, double max) {
		double f = (double)rand() / RAND_MAX;
		return min + f * (max - min);
	}
	static void CreateTestObject(Game * g) {
		//get entityID and add system list to game
		unsigned int index = g->m_entities.add(vector<SystemBase*>{
			&g->m_ts, 
			&g->m_cs,
			&g->m_rs
		});

		//get mesh and bounding box
		MeshStore ms = g->m_cm.GetMeshStore("cone.obj");
		PhysicsComponent pc;
		pc.m_velocity = XMFLOAT3(0, 0, 0);
		pc.m_acceleration = XMFLOAT3(0, 0, 0);
		
		//XMStoreFloat4(&pc.m_rotationalVelocity, XMQuaternionRotationRollPitchYaw(0, 15, 1));
		pc.m_rotationalVelocity = XMFLOAT3(fRand(-30, 30), fRand(-30, 30), fRand(-30, 30));
		//XMStoreFloat3(&pc.m_rotationalAcceleration, XMQuaternionRotationRollPitchYaw(0, 0, 0));
		TransformComponent tc;
		//XMStoreFloat3(&tc.m_rotation, XMQuaternionRotationRollPitchYaw(0, 0, 0));
		tc.m_position = XMFLOAT3(fRand(-100, 100), fRand(5, 100), fRand(-100, 100));
		//tc.m_position = XMFLOAT3(0, 10, -45);
		vector<CollisionType> cTypes = { CollisionType::none };
		//copy collision mask into bounding box
		ms.m_bb.m_cm = MakeCollisionMask(cTypes);

		//create components
		g->m_ts.Create(index, tc, pc);
		g->m_cs.Create(index, ms.m_bb);
		g->m_rs.Create(index, {
			g->m_cm.GetMaterial("TestMaterial"),
			ms.m_m
		});
	}
};