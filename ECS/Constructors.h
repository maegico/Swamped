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
#if BENCHMARK >= 0
		m_renderingComponents["testObj"] = {
			g->m_contentManager.GetMaterial("brickLightingNormalMap"),
			g->m_contentManager.GetMeshStore("fixedTree.obj").m_m
		};
		m_renderingComponents["testObj2"] = {
			g->m_contentManager.GetMaterial("brickLightingNormalMap"),
			g->m_contentManager.GetMeshStore("cube.obj").m_m
		};
#endif
		m_renderingComponents["groundPlane"] = {
			g->m_contentManager.GetMaterial("Ground"),
			g->m_contentManager.GetMeshStore("ground.obj").m_m
		};

		m_renderingComponents["testTree"] = {
			g->m_contentManager.GetMaterial("TreeMaterial"),
			g->m_contentManager.GetMeshStore("fixedTree.obj").m_m
		};
		m_renderingComponents["tree2"] = {
			g->m_contentManager.GetMaterial("TreeMaterial"),
			g->m_contentManager.GetMeshStore("tree2.obj").m_m
		};
		m_renderingComponents["rock"] = {
			g->m_contentManager.GetMaterial("TestMaterial"),
			g->m_contentManager.GetMeshStore("rock1.obj").m_m
		};
		m_renderingComponents["ground"] = {
			g->m_contentManager.GetMaterial("GroundMaterial"),
			g->m_contentManager.GetMeshStore("ground.obj").m_m
		};
		m_renderingComponents["ghost"] = {
			g->m_contentManager.GetMaterial("GhostMaterial"),
			g->m_contentManager.GetMeshStore("ghostDemon.obj").m_m
		};
	}
#if BENCHMARK >= 0
	static void CreateTestObject(Game * game) {
		//get entityID and add system list to game
		EntityId eid = game->m_entities.add(vector<ISystem*>{
			&game->m_transformSystem, 
			&game->m_collisionSystem,
			&game->m_renderingSystem
		});

		//get mesh and bounding box
		MeshStore ms = game->m_contentManager.GetMeshStore("fixedTree.obj");
		PhysicsComponent pc;
		pc.m_velocity = XMFLOAT3(0, 0, 0);
		pc.m_acceleration = XMFLOAT3(0, 0, 0);
		
		//XMStoreFloat4(&pc.m_rotationalVelocity, XMQuaternionRotationRollPitchYaw(0, 15, 1));
		//pc.m_rotationalVelocity = XMFLOAT3(fRand(-30, 30), fRand(-30, 30), fRand(-30, 30));
		//XMStoreFloat3(&pc.m_rotationalAcceleration, XMQuaternionRotationRollPitchYaw(0, 0, 0));
		TransformComponent tc;
		//XMStoreFloat3(&tc.m_rotation, XMQuaternionRotationRollPitchYaw(0, 0, 0));
		tc.m_position = XMFLOAT3(fRand(-100, 100), fRand(0, 100), fRand(-100, 100));
		//tc.m_position = XMFLOAT3(0, 10, -45);
		//vector<CollisionType> cTypes = { CollisionType::none };
		//copy collision mask into bounding box
		ms.m_bb.m_collisionType = CollisionType::test1;

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
		ms.m_bb.m_collisionType = CollisionType::test2;

		//create components
		game->m_transformSystem.Create(eid, tc, pc);
		game->m_collisionSystem.Create(eid, ms.m_bb);
		game->m_renderingSystem.Create(eid, &m_renderingComponents["testObj2"]);
	}
#endif

	//static void CreateGround(Game * game) {
	//	EntityId eid = game->m_entities.add({
	//		&game->m_transformSystem,
	//		&game->m_renderingSystem
	//	});

	//	//MeshStore ms = game->m_contentManager.GetMeshStore("Quad.obj");
	//	PhysicsComponent pc = {};
	//	pc.m_gravity = false;
	//	game->m_transformSystem.Create(eid, {}, pc);
	//	game->m_renderingSystem.Create(eid, &m_renderingComponents["groundPlane"]);
	//}

	static EntityId CreateGhost(Game * g, DirectX::XMFLOAT3 position, float size)
	{
		EntityId eid = g->m_entities.add(vector<ISystem*>{
			&g->m_transformSystem,
				&g->m_collisionSystem,
				&g->m_renderingSystem
		});

		//get mesh and bounding box
		MeshStore ms = g->m_contentManager.GetMeshStore("ghostDemon.obj");

		PhysicsComponent pc;
		pc.m_velocity = XMFLOAT3(0, 0, 0);
		pc.m_acceleration = XMFLOAT3(0, 0, 0);

		TransformComponent tc;
		tc.m_position = position;
		tc.m_scale = size;

		ms.m_bb.m_collisionType = CollisionType::ghost;

		//vector<CollisionType> cTypes = { CollisionType::none };
		//copy collision mask into bounding box
		//ms.m_bb.m_contentManager = MakeCollisionMask(cTypes);

		//create components
		g->m_transformSystem.Create(eid, tc, pc); //transform system
		g->m_collisionSystem.Create(eid, ms.m_bb); //collision system
		g->m_renderingSystem.Create(eid, &m_renderingComponents["ghost"]); //rendering system

		return eid;
	}

	static EntityId CreatePlayer(Game * g)
	{
		EntityId eid = g->m_entities.add(vector<ISystem*>{
			&g->m_transformSystem,
				&g->m_collisionSystem
		});

		//get mesh and bounding box
		MeshStore ms = g->m_contentManager.GetMeshStore("cone.obj");

		PhysicsComponent pc;
		pc.m_velocity = XMFLOAT3(0, 0, 0);
		pc.m_acceleration = XMFLOAT3(0, 0, 0);

		TransformComponent tc;
		//tc.m_position = XMFLOAT3(fRand(-100, 100), fRand(0, 100), fRand(-100, 100));
		tc.m_scale = 1.0f;

		ms.m_bb.m_collisionType = CollisionType::player;

		//vector<CollisionType> cTypes = { CollisionType::none };
		//copy collision mask into bounding box
		//ms.m_bb.m_contentManager = MakeCollisionMask(cTypes);

		//create components
		g->m_transformSystem.Create(eid, tc, pc); //transform system
		g->m_collisionSystem.Create(eid, ms.m_bb); //collision system

		return eid;
	}

	static EntityId CreateTree(Game * g, DirectX::XMFLOAT3 position, float size)
	{
		EntityId eid = g->m_entities.add(vector<ISystem*>{
			&g->m_transformSystem,
				&g->m_collisionSystem,
				&g->m_renderingSystem
		});

		//get mesh and bounding box
		MeshStore ms = g->m_contentManager.GetMeshStore("fixedTree.obj");

		PhysicsComponent pc;
		pc.m_velocity = XMFLOAT3(0, 0, 0);
		pc.m_acceleration = XMFLOAT3(0, 0, 0);

		TransformComponent tc;
		tc.m_position = position;
		tc.m_scale = size;
		//tc.m_rotation = DirectX::XMFLOAT4(0, yRot, 0, 1); 

		//vector<CollisionType> cTypes = { CollisionType::none };
		//copy collision mask into bounding box
		//ms.m_bb.m_contentManager = MakeCollisionMask(cTypes);

		//create components
		g->m_transformSystem.Create(eid, tc, pc); //transform system
		g->m_collisionSystem.Create(eid, ms.m_bb); //collision system
		g->m_renderingSystem.Create(eid, &m_renderingComponents["testTree"]); //rendering system

		return eid;
	}

	static EntityId CreateTree2(Game * g, DirectX::XMFLOAT3 position, float size)
	{
		EntityId eid = g->m_entities.add(vector<ISystem*>{
			&g->m_transformSystem,
				&g->m_collisionSystem,
				&g->m_renderingSystem
		});

		//get mesh and bounding box
		MeshStore ms = g->m_contentManager.GetMeshStore("tree2.obj");

		PhysicsComponent pc;
		pc.m_velocity = XMFLOAT3(0, 0, 0);
		pc.m_acceleration = XMFLOAT3(0, 0, 0);

		TransformComponent tc;
		tc.m_position = position;
		tc.m_scale = size;

		//vector<CollisionType> cTypes = { CollisionType::none };
		//copy collision mask into bounding box
		//ms.m_bb.m_contentManager = MakeCollisionMask(cTypes);

		//create components
		g->m_transformSystem.Create(eid, tc, pc); //transform system
		g->m_collisionSystem.Create(eid, ms.m_bb); //collision system
		g->m_renderingSystem.Create(eid, &m_renderingComponents["tree2"]); //rendering system

		return eid;
	}

	static EntityId CreateRock(Game * g, DirectX::XMFLOAT3 position, float size)
	{
		EntityId eid = g->m_entities.add(vector<ISystem*>{
			&g->m_transformSystem,
				&g->m_collisionSystem,
				&g->m_renderingSystem
		});

		//get mesh and bounding box
		MeshStore ms = g->m_contentManager.GetMeshStore("rock1.obj");

		PhysicsComponent pc;
		pc.m_velocity = XMFLOAT3(0, 0, 0);
		pc.m_acceleration = XMFLOAT3(0, 0, 0);

		TransformComponent tc;
		tc.m_position = position;
		tc.m_scale = size;

		//vector<CollisionType> cTypes = { CollisionType::none };
		//copy collision mask into bounding box
		//ms.m_bb.m_contentManager = MakeCollisionMask(cTypes);

		//create components
		g->m_transformSystem.Create(eid, tc, pc); //transform system
		g->m_collisionSystem.Create(eid, ms.m_bb); //collision system
		g->m_renderingSystem.Create(eid, &m_renderingComponents["rock"]); //rendering system

		return eid;
	}

	static EntityId CreateGround(Game * g, DirectX::XMFLOAT3 position, float size)
	{
		EntityId eid = g->m_entities.add(vector<ISystem*>{
			&g->m_transformSystem,
				&g->m_collisionSystem,
				&g->m_renderingSystem
		});

		//get mesh and bounding box
		MeshStore ms = g->m_contentManager.GetMeshStore("ground.obj");

		PhysicsComponent pc;
		pc.m_velocity = XMFLOAT3(0, 0, 0);
		pc.m_acceleration = XMFLOAT3(0, 0, 0);

		TransformComponent tc;
		tc.m_position = position;
		tc.m_scale = size;


		//vector<CollisionType> cTypes = { CollisionType::none };
		//copy collision mask into bounding box
		//ms.m_bb.m_contentManager = MakeCollisionMask(cTypes);

		//create components
		g->m_transformSystem.Create(eid, tc, pc); //transform system
		g->m_collisionSystem.Create(eid, ms.m_bb); //collision system
		g->m_renderingSystem.Create(eid, &m_renderingComponents["ground"]); //rendering system

		return eid;
	}
};