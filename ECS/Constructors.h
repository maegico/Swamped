#pragma once
#include "Game.h"
#include "ContentManager.h"
#include "RenderingComponent.h"
#include "GlobalFunctions.h"
#include <unordered_map>

using namespace DirectX;
//Contains static constructors for preformed entities
class Constructors {
	static unordered_map<std::string, RenderingComponent> m_renderingComponents;
public:
	static void Init(Game * g) {
		m_renderingComponents["testObj"] = {
			g->m_contentManager.GetMaterial("brickLightingNormalMap"),
			g->m_contentManager.GetMeshStore("cone.obj").m_m
		};
		m_renderingComponents["testObj2"] = {
			g->m_contentManager.GetMaterial("brickLightingNormalMap"),
			g->m_contentManager.GetMeshStore("cube.obj").m_m
		};
	}
	static void CreateTestObject(Game * game) {
		//get entityID and add system list to game
		EntityId eid = game->m_entities.add(vector<ISystem*>{
			&game->m_transformSystem, 
			&game->m_collisionSystem,
			&game->m_renderingSystem
		});

		//get mesh and bounding box
		MeshStore ms = game->m_contentManager.GetMeshStore("cone.obj");
		PhysicsComponent pc;
		pc.m_velocity = XMFLOAT3(0, 0, 0);
		pc.m_acceleration = XMFLOAT3(0, 0, 0);
		
		//XMStoreFloat4(&pc.m_rotationalVelocity, XMQuaternionRotationRollPitchYaw(0, 15, 1));
		pc.m_rotationalVelocity = XMFLOAT3(fRand(-30, 30), fRand(-30, 30), fRand(-30, 30));
		//XMStoreFloat3(&pc.m_rotationalAcceleration, XMQuaternionRotationRollPitchYaw(0, 0, 0));
		TransformComponent tc;
		//XMStoreFloat3(&tc.m_rotation, XMQuaternionRotationRollPitchYaw(0, 0, 0));
		tc.m_position = XMFLOAT3(fRand(-100, 100), fRand(0, 100), fRand(-100, 100));
		//tc.m_position = XMFLOAT3(0, 10, -45);
		//vector<CollisionType> cTypes = { CollisionType::none };
		//copy collision mask into bounding box
		ms.m_bb.m_collisionType = CollisionType::none;

		//create components
		game->m_transformSystem.Create(eid, tc, pc);
		game->m_collisionSystem.Create(eid, ms.m_bb);
		game->m_renderingSystem.Create(eid, &m_renderingComponents["testObj"]);
	}

	static void CreateTestObject2(Game * game) {
		//get entityID and add system list to game
		EntityId eid = game->m_entities.add(vector<ISystem*>{
			&game->m_transformSystem,
			&game->m_collisionSystem,
			&game->m_renderingSystem
		});

		//get mesh and bounding box
		MeshStore ms = game->m_contentManager.GetMeshStore("cube.obj");
		PhysicsComponent pc;
		pc.m_velocity = XMFLOAT3(0, 0, 0);
		pc.m_acceleration = XMFLOAT3(0, 0, 0);

		//XMStoreFloat4(&pc.m_rotationalVelocity, XMQuaternionRotationRollPitchYaw(0, 15, 1));
		pc.m_rotationalVelocity = XMFLOAT3(fRand(-30, 30), fRand(-30, 30), fRand(-30, 30));
		//XMStoreFloat3(&pc.m_rotationalAcceleration, XMQuaternionRotationRollPitchYaw(0, 0, 0));
		TransformComponent tc;
		//XMStoreFloat3(&tc.m_rotation, XMQuaternionRotationRollPitchYaw(0, 0, 0));
		tc.m_position = XMFLOAT3(fRand(-100, 100), fRand(0, 100), fRand(-100, 100));
		//tc.m_position = XMFLOAT3(0, 10, -45);
		//vector<CollisionType> cTypes = { CollisionType::none };
		//copy collision mask into bounding box
		ms.m_bb.m_collisionType = CollisionType::player;

		//create components
		game->m_transformSystem.Create(eid, tc, pc);
		game->m_collisionSystem.Create(eid, ms.m_bb);
		game->m_renderingSystem.Create(eid, &m_renderingComponents["testObj2"]);
	}
};