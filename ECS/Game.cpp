#include "Game.h"
#include "Constructors.h"

Game::Game(HINSTANCE hInstance) 
	: DXCore(
	hInstance,		   // The application's handle
	"DirectX Game",			   // Height of the window's client area
	true)			   // Show extra stats (fps) in title bar?
{
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

void Game::Init() {
	m_cm.Init(m_device, m_context);
	m_rs.Init(m_swapChain, m_device, m_context, m_backBufferRTV, m_depthStencilView);

	Constructors::CreateTestObject(this);
	/*PhysicsComponent pc;
	pc.m_velocity = XMFLOAT3(0, 0, 0);
	pc.m_acceleration = XMFLOAT3(0, 0, 0);
	XMStoreFloat4(&pc.m_rotationalVelocity, XMQuaternionRotationRollPitchYaw(1, 0, 0));
	XMStoreFloat4(&pc.m_rotationalAcceleration, XMQuaternionRotationRollPitchYaw(0, 0, 0));
	TransformComponent tc;
	tc.m_position = XMFLOAT3((rand() % 50) - 25, (rand() % 50) - 25, (rand() % 50) - 25);
	Constructors::CreateTransform(this, tc, pc, {
		{
			{ -1,-1,-1 },
			{ -1,-1,1 },
			{ -1,1,-1 },
			{ -1,1,1 },
			{ 1,-1,-1 },
			{ 1,-1,1 },
			{ 1,1,-1 },
			{ 1,1,1 }
		},
		CollisionType::none
	});*/
	//Constructors::CreateTransform(this, {}, {}, {});
}

//delete system objects
Game::~Game() {
}

//Advances the game in time
void Game::Update(float dt, float totalTime) {
	m_ts.Update(this, dt);
	m_cs.Update(this, dt);
	m_rs.Update(this, dt);

	//remove all entities queued for removal
	for (unsigned int eId : m_removeQueue) {
		//call remove on the entity ID for each system associated with the entity
		vector<SystemBase*> systems = m_entities[eId];
		for (SystemBase* s : systems) {
			s->Remove(eId);
		}
		//free the ID in entity list
		m_entities.free(eId);
	}
	m_removeQueue.clear();
}

//Removes an entity from all its systems
void Game::QueueRemoveEntity(unsigned int entityId) {
	//bounds check
	if (m_entities.size() > entityId)
	{
		m_removeQueue.push_back(entityId);
	}
}

void Game::UpdateTitleBarForGame(std::string in) {
	SetWindowText(hWnd, (in + " " + std::to_string(m_ts.GetCount())).c_str());
}