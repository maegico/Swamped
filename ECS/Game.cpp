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
	m_contentManager.Init(m_device, m_context);
	m_renderingSystem.Init(this, m_swapChain, m_device, m_context, m_backBufferRTV, m_depthStencilView);
	Constructors::Init(this);
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
	unsigned int newComponents = 100;
	for (unsigned int c = 0; c < newComponents; c++)
	{
		Constructors::CreateTestObject(this);
		Constructors::CreateTestObject2(this);
	}
	m_transformSystem.Update(this, dt);
	m_particleSystem.Update(this, dt);
	m_collisionSystem.Update(this, dt);
	m_renderingSystem.Update(this, dt);
	//std::cout << std::to_string(m_removeQueue.size()) << std::endl;
	//remove all entities queued for removal
	for (unsigned int eId : m_removeQueue) {
		//call remove on the entity ID for each system associated with the entity
		vector<ISystem*> systems = m_entities[eId];
		for (ISystem* s : systems) {
			s->Remove(eId);
		}
		//free the ID in entity list
		m_entities.free(eId);
	}
	m_removeQueue.clear();
}

//Removes an entity from all its systems
void Game::QueueRemoveEntity(EntityId entityId) {
	//bounds check
	if (m_entities.size() > entityId)
	{
		m_removeQueue.add(entityId);
	}
}

void Game::UpdateTitleBarForGame(std::string in) {
	XMFLOAT3 cellCounts = m_collisionSystem.GetCellCounts();
	SetWindowText(hWnd, (in + "  Objects: " + std::to_string(m_transformSystem.GetCount()) + "  Particles: " + std::to_string(m_particleSystem.GetParticleCount()) + " Cell Counts: " + std::to_string(cellCounts.x) +", "+std::to_string(cellCounts.y)+", "+std::to_string(cellCounts.z)).c_str());
}