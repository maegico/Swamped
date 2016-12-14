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
	Constructors::CreateGround(this);
	m_toggles.push_back(Toggle('A', &m_renderingSystem.m_fxaaToggle));
	m_toggles.push_back(Toggle('B', &m_renderingSystem.m_bloomToggle));
}

//delete system objects
Game::~Game() {
}

//Advances the game in time
void Game::Update(float dt, float totalTime) {
	for (Toggle& t : m_toggles)
		t.Check();

	if (dt > m_timeStep * 4)
		dt = m_timeStep;

	m_accumulator += dt;

	while (m_accumulator >= m_timeStep) {
#if BENCHMARK >= 0
#ifdef _DEBUG
		unsigned int newComponents = 100 * m_timeStep;
#else
		unsigned int newComponents = BENCHMARK * 1000 * m_timeStep;
#endif
		for (unsigned int c = 0; c < newComponents; c++)
		{
			Constructors::CreateTestObject(this);
			Constructors::CreateTestObject2(this);
		}
#endif

		m_transformSystem.Update(this, m_timeStep);
		m_collisionSystem.Update(this, m_timeStep);
		m_accumulator -= m_timeStep;
	}

	m_particleSystem.Update(this, dt, totalTime);
	m_renderingSystem.Update(this, dt, totalTime);

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
	double totalUpdateTime = m_collisionSystem.GetTotalTime() + m_particleSystem.GetTotalTime() + m_renderingSystem.GetTotalTime() + m_transformSystem.GetTotalTime();
	XMFLOAT3 cellCounts = m_collisionSystem.GetCellCounts();
	SetWindowText(hWnd, 
		(in + 
			" Objects: " + std::to_string(m_transformSystem.GetCount()) + 
			" Particles: " + std::to_string(m_particleSystem.GetParticleCount()) + 
			" Cell Divisions: " + std::to_string(static_cast<int>(cellCounts.x)) + 
			" FXAA: "+std::to_string(m_renderingSystem.m_fxaaToggle) + 
			" Bloom: "+std::to_string(m_renderingSystem.m_bloomToggle) + 
			" Collisions: "+std::to_string((m_collisionSystem.GetTotalTime()/totalUpdateTime)) +
			" Particles: " + std::to_string((m_particleSystem.GetTotalTime() / totalUpdateTime)) +
			" Rendering: " + std::to_string((m_renderingSystem.GetTotalTime() / totalUpdateTime)) +
			" Transforms: " + std::to_string((m_transformSystem.GetTotalTime() / totalUpdateTime))
		).c_str());
}