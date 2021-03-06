#pragma once

#define BENCHMARK -1

#include "CollisionSystem.h"
#include "TransformSystem.h"
#include "RenderingSystem.h"
#include "ParticleSystem.h"
#include "ContentManager.h"
#include "DXCore.h"
#include "EntityIdTypeDef.h"
#include "Toggle.h"
#include "Ghost.h"
#include <vector>
#include <chrono>
#include <math.h>

using namespace std;
using namespace std::chrono;

//The game instance. Not a singleton
class Game : public DXCore {
public:
	//Systems
	CollisionSystem m_collisionSystem;
	TransformSystem m_transformSystem;
	RenderingSystem m_renderingSystem;
	ParticleSystem m_particleSystem;

	Ghost m_ghost;
	Ghost m_ghost2;

	EntityId m_playerId;

	ContentManager m_contentManager;

	Game(HINSTANCE hInstance);
	~Game();

	void Init();

	//Update for the game. The program's main loop will call this
	void Update(float dT, float totalTime);

	//Frees an entity's components, and frees its entry in m_entities
	void QueueRemoveEntity(EntityId entityId);

	POINT prevMousePos;
	void OnMouseDown(WPARAM buttonState, int x, int y);
	void OnMouseUp(WPARAM buttonState, int x, int y);
	void OnMouseMove(WPARAM buttonState, int x, int y);

	friend class Constructors;
private:
	//Associates systems with entity IDs for deletion
	FreeVector<vector<ISystem*>> m_entities;
	ClearVector<EntityId> m_removeQueue;
	void UpdateTitleBarForGame(std::string in);

	vector<Toggle> m_toggles;

	float m_accumulator = 0;
	const float m_timeStep = 1.0 / 60;

	void OnResize();

	XMFLOAT2 m_playerRotation = XMFLOAT2(0, 0);

	//bool fxaaToggle = false;
};
